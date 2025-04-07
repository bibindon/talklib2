#include "talklib2.h"
#include <sstream>
#include "HeaderOnlyCsv.hpp"
#include "CaesarCipher.h"

using namespace NSTalkLib2;

bool Talk::m_fastMode = false;

static std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

void Talk::Init(const std::string& csvfilepath,
                IFont* font,
                ISoundEffect* SE,
                ISprite* sprite,
                const std::string& textBackImgPath,
                const std::string& blackImgPath,
                const bool encrypt)
{
    m_csvfilepath = csvfilepath;
    m_font = font;
    m_SE = SE;
    m_sprite = sprite;
    m_sprTextBack = sprite->Create();
    m_sprFade = sprite->Create();
    m_encrypt = encrypt;

    m_font->Init();
    m_SE->Init();
    m_sprTextBack->Load(textBackImgPath);
    m_sprFade->Load(blackImgPath);

    m_isFadeIn = true;

    std::vector<TalkBall*> talkList = CreateTalkList();
    m_talkBallList = talkList;
}

void NSTalkLib2::Talk::UpdateConstValue()
{
    if (m_fastMode)
    {
        fade_frame_max = 1;
        wait_next_frame = 1;
    }
    else
    {
        fade_frame_max = FADE_FRAME_MAX;
        wait_next_frame = WAIT_NEXT_FRAME;
    }
}

std::vector<TalkBall*> Talk::CreateTalkList()
{
    std::vector<TalkBall*> talkList;

    std::vector<std::vector<std::string> > vss;

    if (m_encrypt == false)
    {
        vss = csv::Read(m_csvfilepath);
    }
    else
    {
        auto workStr = CaesarCipher::DecryptFromFile(m_csvfilepath);
        vss = csv::ReadFromString(workStr);
    }

    for (std::size_t i = 1; i < vss.size(); ++i)
    {
        TalkBall* talkBall = new TalkBall();
        talkBall->Init(vss.at(i), m_font, m_sprTextBack, m_SE);
        talkList.push_back(talkBall);
    }
    return talkList;
}

void Talk::Next()
{
    if (m_waitNextCount < wait_next_frame)
    {
        return;
    }

    if (m_talkBallList.at(m_talkBallIndex)->IsFinish() == false)
    {
        return;
    }

    if (m_talkBallIndex < (int)m_talkBallList.size() - 1)
    {
        m_talkBallIndex++;
    }
    else
    {
        m_FadeOutCount = 0;
        m_isFadeOut = true;
    }
    m_waitNextCount = 0;

}

// 戻り値は会話終了フラグ
bool Talk::Update()
{
    UpdateConstValue();

    bool isFinish = false;
    m_waitNextCount++;
    if (m_isFadeIn)
    {
        if (m_FadeInCount < fade_frame_max)
        {
            m_FadeInCount++;
        }
        else
        {
            m_isFadeIn = false;
            m_FadeInCount = 0;
        }
    }
    if (m_isFadeOut)
    {
        if (m_FadeOutCount < fade_frame_max)
        {
            m_FadeOutCount++;
        }
        else
        {
            isFinish = true;
        }
    }
    m_talkBallList.at(m_talkBallIndex)->Update(m_fastMode);

    return isFinish;
}

void Talk::Render()
{
    m_talkBallList.at(m_talkBallIndex)->Render();

    if (m_isFadeIn)
    {
        m_sprFade->DrawImage(0, 0, 255 - m_FadeInCount*255/fade_frame_max);
    }
    if (m_isFadeOut)
    {
        m_sprFade->DrawImage(0, 0, m_FadeOutCount*255/fade_frame_max);
    }
}

void NSTalkLib2::Talk::SetFastMode(const bool arg)
{
    m_fastMode = arg;
}

Talk::~Talk()
{
    for (size_t i = 0; i < m_talkBallList.size(); ++i)
    {
        delete m_talkBallList.at(i);
        m_talkBallList.at(i) = nullptr;
    }

    delete m_sprFade;
    m_sprFade = nullptr;

    delete m_sprTextBack;
    m_sprTextBack = nullptr;

    delete m_sprite;
    m_sprite = nullptr;

    delete m_SE;
    m_SE = nullptr;

    delete m_font;
    m_font = nullptr;
}

void TalkBall::Init(const std::vector<std::string>& csvOneLine,
                    IFont* font,
                    ISprite* sprite,
                    ISoundEffect* SE)
{
    m_font = font;
    m_spriteBack = sprite;
    m_SE = SE;
    m_spriteLeft = sprite->Create();
    m_spriteRight = sprite->Create();

    std::vector<std::string> vs;

    std::string work = csvOneLine.at(1);
    work.erase(remove(work.begin(), work.end(), '\"'), work.end());
    vs = split(work, '\n');
    m_text = vs;
    m_text.resize(3);

    work = csvOneLine.at(2);
    if (work.empty() == false)
    {
        m_spriteLeft->Load(work);
    }

    work = csvOneLine.at(3);
    if (work.empty() == false)
    {
        m_spriteRight->Load(work);
    }

    m_textShow.resize(3);
}

void TalkBall::Update(const bool fastmode)
{
    m_textIndex++;

    bool finish = false;

    // 文字送り処理
    m_textShow.at(0).clear();
    m_textShow.at(1).clear();
    m_textShow.at(2).clear();

    // 30フレーム経過してから文字の表示を始める
    m_counter++;

    if (!fastmode)
    {
        if (m_counter < 30)
        {
            return;
        }
    }
    else
    {
        if (m_counter < 1)
        {
            return;
        }
    }

    if (m_isSEPlay == false)
    {
        m_isSEPlay = true;
        m_SE->PlayMessage();
    }

    if (!fastmode)
    {
        m_charCount += 3;
    }
    else
    {
        m_charCount += 300;
    }

    // 一行目
    if (m_charCount < (int)m_text.at(0).size())
    {
        // マルチバイト文字は1文字で2バイトであることを考慮する
        if (m_charCount % 2 == 0)
        {
            m_textShow.at(0) = m_text.at(0).substr(0, (size_t)m_charCount);
        }
        else
        {
            m_textShow.at(0) = m_text.at(0).substr(0, (size_t)m_charCount - 1);
        }
    }
    else
    {
        m_textShow.at(0) = m_text.at(0);
    }

    int total = 0;

    // 二行目
    total = (int)m_text.at(0).size() + (int)m_text.at(1).size();
    int secondLineCount = m_charCount - (int)m_text.at(0).size();
    if (m_charCount < total)
    {
        if (secondLineCount >= 0)
        {
            // マルチバイト文字は1文字で2バイトであることを考慮する
            if (secondLineCount % 2 == 0)
            {
                m_textShow.at(1) = m_text.at(1).substr(0, secondLineCount);
            }
            else
            {
                m_textShow.at(1) = m_text.at(1).substr(0, (size_t)secondLineCount - 1);
            }
        }
    }
    else
    {
        m_textShow.at(1) = m_text.at(1);
    }

    // 三行目
    total = (int)m_text.at(0).size() + (int)m_text.at(1).size() + (int)m_text.at(2).size();

    int thirdLineCount = m_charCount - (int)m_text.at(0).size() - (int)m_text.at(1).size();
    if (m_charCount < total)
    {
        if (thirdLineCount >= 0)
        {
            // マルチバイト文字は1文字で2バイトであることを考慮する
            if (thirdLineCount % 2 == 0)
            {
                m_textShow.at(2) = m_text.at(2).substr(0, thirdLineCount);
            }
            else
            {
                m_textShow.at(2) = m_text.at(2).substr(0, (size_t)(thirdLineCount - 1));
            }
        }
    }
    else
    {
        m_textShow.at(2) = m_text.at(2);
        m_isFinish = true;
        m_SE->Stop();
    }

}

void TalkBall::Render()
{
    if (m_spriteLeft != nullptr)
    {
        m_spriteLeft->DrawImage(0, 0);
    }

    if (m_spriteRight != nullptr)
    {
        m_spriteRight->DrawImage(700, 0);
    }

    m_spriteBack->DrawImage(0, 0);

    if (m_textShow.size() >= 1)
    {
        m_font->DrawText_(m_textShow.at(0), 100, 730);
    }

    if (m_textShow.size() >= 2)
    {
        m_font->DrawText_(m_textShow.at(1), 100, 780);
    }

    if (m_textShow.size() >= 3)
    {
        m_font->DrawText_(m_textShow.at(2), 100, 830);
    }
}

bool TalkBall::IsFinish()
{
    return m_isFinish;
}

TalkBall::~TalkBall()
{
    delete m_spriteLeft;
    m_spriteLeft = nullptr;

    delete m_spriteRight;
    m_spriteRight = nullptr;
}

