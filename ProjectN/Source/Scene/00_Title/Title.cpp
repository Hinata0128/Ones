#include "Title.h"

#include "SceneManager/SceneManager.h"
#include "GameObject/UIObject/UIObject.h"
#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include "System/02_Singleton/00_Timer/Timer.h"

Title::Title()
    : m_Select(SelectMenu::Start)
    , m_State(TitleState::Select)
    , m_StartPos(20.f, 0.f, 0.f)
    , m_EndPos(0.f, 50.f, 0.f)
    , m_InputTimer(0.0f)
    , m_FadeAlpha(0.0f)
    , m_FadeSpeed(1.5f)
{
    m_pSpriteTitle = std::make_shared<Sprite2D>();
    m_upTitle = std::make_shared<UIObject>();

    m_pSpriteStart = std::make_shared<Sprite2D>();
    m_upStart = std::make_shared<UIObject>();

    m_pSpriteEnd = std::make_shared<Sprite2D>();
    m_upEnd = std::make_shared<UIObject>();

    m_pSpriteFade = std::make_shared<Sprite2D>();
    m_upFade = std::make_shared<UIObject>();
}

Title::~Title()
{
}

void Title::Initialize()
{
}

void Title::Create()
{
    const float W = 1280.0f;
    const float H = 720.0f;

    // 背景画像の設定
    Sprite2D::SPRITE_STATE ssTitle{ W,H,W,H,W,H };
    m_pSpriteTitle->Init(_T("Data\\Image\\Setting\\Surface.png"), ssTitle);
    m_upTitle->AttachSprite(m_pSpriteTitle);
    // ★追加: 背景を画面中央（または左上）に配置
    m_upTitle->SetPosition({ 0.0f, 0.0f, 0.0f });

    // スタートボタン
    Sprite2D::SPRITE_STATE ssStart{ 135,45,135,45,135,45 };
    m_pSpriteStart->Init(_T("Data\\Image\\Setting\\S_Start.png"), ssStart);
    m_upStart->AttachSprite(m_pSpriteStart);

    // エンドボタン
    Sprite2D::SPRITE_STATE ssEnd{ 84,45,84,45,84,45 };
    m_pSpriteEnd->Init(_T("Data\\Image\\Setting\\S_End.png"), ssEnd);
    m_upEnd->AttachSprite(m_pSpriteEnd);

    // フェード用の黒画像
    Sprite2D::SPRITE_STATE ssFade{ W,H,W,H,W,H };
    m_pSpriteFade->Init(_T("Data\\Image\\Setting\\Black.png"), ssFade);
    m_upFade->AttachSprite(m_pSpriteFade);
    // ★追加: フェード用パネルも画面全体を覆う位置に配置
    m_upFade->SetPosition({ 0.0f, 0.0f, 0.0f });
    m_upFade->SetAlpha(0.0f);
}

void Title::Update()
{
    switch (m_State)
    {
    case TitleState::Select:
        UpdateSelect();
        break;

    case TitleState::FadeOut:
        UpdateFadeOut();
        break;

    case TitleState::End:
        SceneManager::GetInstance()->LoadScene(SceneManager::First);
        break;
    }
}

void Title::Draw()
{
    auto dx = DirectX11::GetInstance();

    dx->SetDepth(false);
    dx->SetAlphaBlend(true);

    // 1. 背景を描画
    m_upTitle->Draw();

    // 2. ボタンの座標更新と描画
    m_upStart->SetPosition(m_StartPos);
    m_upEnd->SetPosition(m_EndPos);
    m_upStart->Draw();
    m_upEnd->Draw();

    // 3. フェード（常に最前面）
    // ステートに関わらず、アルファ値が0より大きければ描画する
    if (m_FadeAlpha > 0.0f)
    {
        m_upFade->SetAlpha(m_FadeAlpha);
        m_upFade->Draw();
    }

    dx->SetAlphaBlend(false);
    dx->SetDepth(true);
}

void Title::UpdateSelect()
{
    m_InputTimer += Timer::GetInstance().DeltaTime();
    if (m_InputTimer < 0.2f) return;

    if (GetAsyncKeyState(VK_UP) & 0x0001)
    {
        m_Select = SelectMenu::Start;
        m_InputTimer = 0.0f;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x0001)
    {
        m_Select = SelectMenu::End;
        m_InputTimer = 0.0f;
    }

    if (m_Select == SelectMenu::Start)
    {
        m_StartPos = { 20.f, 0.f, 0.f };
        m_EndPos = { 0.f, 50.f, 0.f };
    }
    else
    {
        m_StartPos = { 0.f, 0.f, 0.f };
        m_EndPos = { 20.f, 50.f, 0.f };
    }

    if (GetAsyncKeyState(VK_RETURN) & 0x0001)
    {
        if (m_Select == SelectMenu::Start)
        {
            m_State = TitleState::FadeOut;
            m_FadeAlpha = 0.0f;
        }
        else
        {
            PostQuitMessage(0);
        }
    }
}

void Title::UpdateFadeOut()
{
    m_FadeAlpha += m_FadeSpeed * Timer::GetInstance().DeltaTime();

    if (m_FadeAlpha >= 1.0f)
    {
        m_FadeAlpha = 1.0f;
        m_State = TitleState::End;
    }
}
