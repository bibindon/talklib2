// ノベルベームのようにキャラクターの画像が表示される会話機能
#pragma once
#include <string>
#include <vector>

class ISprite
{
public:
    virtual void DrawImage(const int x, const int y, const int transparency = 255) = 0;
    virtual void Load(const std::string& filepath) = 0;
    virtual ~ISprite() {};
    virtual ISprite* Create() = 0; // 仮想コンストラクタ
};

class IFont
{
public:
    virtual void DrawText_(const std::string& msg, const int x, const int y) = 0;
    virtual void Init() = 0;
    virtual ~IFont() {};
};

class ISoundEffect
{
public:
    virtual void PlayMessage() = 0;
    virtual void Stop() = 0;
    virtual void Init() = 0;
    virtual ~ISoundEffect() {};
};

class TalkBall
{
public:
    void Init(const std::vector<std::string>& csvOneLine,
              IFont* font,
              ISprite* sprite,
              ISoundEffect* SE);
    void Update();
    void Render();
    bool IsFinish();
    void Finalize();

private:
    std::vector<std::string> m_textShow;
    std::vector<std::string> m_text;
    int m_textIndex = 0;
    int m_counter = 0;
    int m_charCount = 0;
    IFont* m_font = nullptr;
    ISprite* m_spriteBack = nullptr;
    ISprite* m_spriteLeft = nullptr;
    ISprite* m_spriteRight = nullptr;
    ISoundEffect* m_SE = nullptr;

    bool m_isSEPlay = false;
    bool m_isFinish = false;
};

class Talk
{
public:

    void Init(const std::string& csvFilename,
              IFont* font,
              ISoundEffect* SE,
              ISprite* sprite);
    void Next();
    bool Update();
    void Render();
    void Finalize();

private:
    std::vector<TalkBall> CreateTalkList();

    std::string m_csvfilepath;
    ISprite* m_sprite;
    ISprite* m_sprTextBack;
    IFont* m_font;
    ISoundEffect* m_SE;
    std::vector<TalkBall> m_talkBallList;
    int m_talkBallIndex = 0;

    ISprite* m_sprFade;
    // 30フレームかけて表示する。
    // 30フレームではなく500ミリ秒、でやるべきだが、それほど大きな問題とならないのでよしとする。
    const int FADE_FRAME_MAX = 30;
    bool m_isFadeIn = false;
    int m_FadeInCount = 0;
    bool m_isFadeOut = false;
    int m_FadeOutCount = 0;

    const int WAIT_NEXT_FRAME = 60;
    int m_waitNextCount = 0;

};

