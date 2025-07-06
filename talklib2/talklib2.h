//---------------------------------------------------------
// ノベルベームのようにキャラクターの画像が表示される会話機能
//---------------------------------------------------------
#pragma once
#include <string>
#include <vector>

namespace NSTalkLib2
{
class ISprite
{
public:
    virtual void DrawImage(const int x, const int y, const int transparency = 255) = 0;
    virtual void Load(const std::wstring& filepath) = 0;
    virtual ~ISprite() {};
    virtual ISprite* Create() = 0; // 仮想コンストラクタ
};

class IFont
{
public:
    virtual void DrawText_(const std::wstring& msg, const int x, const int y) = 0;
    virtual void Init(const bool bEnglish) = 0;
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

class IBGM
{
public:
    virtual void Init(const std::wstring& filepath) = 0;
    virtual void Finalize() = 0;
    virtual void Play() = 0;
    virtual ~IBGM() {};
};

class TalkBall
{
public:
    ~TalkBall();

    void Init(const std::vector<std::wstring>& csvOneLine,
              IFont* font,
              ISprite* sprite,
              ISoundEffect* SE,
              IBGM* bgm);

    void Update(const bool fastmode);
    void Render();
    bool IsFinish();

private:
    std::vector<std::wstring> m_textShow;
    std::vector<std::wstring> m_text;
    int m_textIndex = 0;
    int m_counter = 0;
    int m_charCount = 0;
    IFont* m_font = nullptr;

    // フェードインアウト用の黒画像
    ISprite* m_spriteFade = nullptr;

    ISprite* m_spriteLeft = nullptr;
    ISprite* m_spriteRight = nullptr;

    // 背景画像
    ISprite* m_spriteBack = nullptr;

    ISoundEffect* m_SE = nullptr;
    IBGM* m_BGM = nullptr;

    bool m_isSEPlay = false;
    bool m_isFinish = false;
};

class Talk
{
public:

    ~Talk();

    void Init(const std::wstring& csvFilename,
              IFont* font,
              ISoundEffect* SE,
              ISprite* sprite,
              const std::wstring& textBackImgPath,
              const std::wstring& blackImgPath,
              const bool encrypt,
              const bool bEnglish,
              IBGM* bgm = nullptr);

    void Next();
    bool Update();
    void Render();

    static void SetFastMode(const bool arg);

private:

    static bool m_fastMode;
    void UpdateConstValue();

    std::vector<TalkBall*> CreateTalkList();

    std::wstring m_csvfilepath;
    ISprite* m_sprite = nullptr;
    ISprite* m_sprTextBack = nullptr;
    IFont* m_font = nullptr;
    ISoundEffect* m_SE = nullptr;
    IBGM* m_BGM = nullptr;

    bool m_encrypt = false;
    std::vector<TalkBall*> m_talkBallList;
    int m_talkBallIndex = 0;

    ISprite* m_sprFade = nullptr;

    // 30フレームかけて表示する。
    // 30フレームではなく500ミリ秒、でやるべきだが、それほど大きな問題とならないのでよしとする。
    const int FADE_FRAME_MAX = 30;
    int fade_frame_max = FADE_FRAME_MAX;

    bool m_isFadeIn = false;
    int m_FadeInCount = 0;
    bool m_isFadeOut = false;
    int m_FadeOutCount = 0;

    const int WAIT_NEXT_FRAME = 30;
    int wait_next_frame = WAIT_NEXT_FRAME;

    int m_waitNextCount = 0;

    bool m_bEnglish = false;

};
}

