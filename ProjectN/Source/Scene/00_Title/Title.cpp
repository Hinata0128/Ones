#include "Title.h"

#include "SceneManager/SceneManager.h"
#include "GameObject/03_UIObject/UIObject.h"
#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include "System/02_Singleton/00_Timer/Timer.h"

Title::Title()
    : m_Select              ( SelectMenu::Start )
    , m_State               ( TitleState::Select )
    , m_StartPos            ( 550.f, 550.f, 0.f )
    , m_EndPos              ( 550.f, 650.f, 0.f )
    , m_InputTimer          ( 0.0f )
    , m_FadeAlpha           ( 0.0f )
    , m_FadeSpeed           ( 1.5f )

    , m_pSpriteTitle        ( std::make_shared<Sprite2D>() )
    , m_upTitle             ( std::make_shared<UIObject>() )

    ,m_pSpriteStart         ( std::make_shared<Sprite2D>() )
    ,m_upStart              ( std::make_shared<UIObject>() )

    ,m_pSpriteEnd           ( std::make_shared<Sprite2D>() )
    ,m_upEnd                ( std::make_shared<UIObject>() )

    ,m_pSpriteFade          ( std::make_shared<Sprite2D>() )
    ,m_upFade               ( std::make_shared<UIObject>() )

    ,m_pSpriteSelectBack    ( std::make_shared<Sprite2D>() )
    ,m_upSelectBack         ( std::make_shared<UIObject>() )

    ,m_pSpriteSelectFrame   ( std::make_shared<Sprite2D>() )
    ,m_upSelectFrame        ( std::make_shared<UIObject>() )

{
}

Title::~Title()
{
}

void Title::Initialize()
{
}

void Title::Create()
{
    //タイトル画像のサイズ.
    const float WND_W = 1280.0f;
    const float WND_H = 720.0f;

    //スタート画像のサイズ.
    const float Start_W = 135.0f;
    const float Staet_H = 45.0f;

    //エンド画像のサイズ.
    const float End_W = 84.0f;
    const float End_H = 45.0f;

    //背景画像の設定
    Sprite2D::SPRITE_STATE ssTitle{ WND_W, WND_H, WND_W, WND_H, WND_W, WND_H };
    //背景画像の読み込み.
    m_pSpriteTitle->Init(_T("Data\\Image\\Setting\\Surface.png"), ssTitle);
    m_upTitle->AttachSprite(m_pSpriteTitle);
    m_upTitle->SetPosition( 0.0f, 0.0f, 0.0f );

    // スタートボタン
    Sprite2D::SPRITE_STATE ssStart{ Start_W, Staet_H, Start_W, Staet_H, Start_W, Staet_H };
    //スタート画像の読み込み.
    m_pSpriteStart->Init(_T("Data\\Image\\Setting\\S_Start.png"), ssStart);
    m_upStart->AttachSprite(m_pSpriteStart);

    // エンドボタン
    Sprite2D::SPRITE_STATE ssEnd{ End_W, End_H, End_W, End_H, End_W, End_H };
    //エンド画像の読み込み.
    m_pSpriteEnd->Init(_T("Data\\Image\\Setting\\S_End.png"), ssEnd);
    m_upEnd->AttachSprite(m_pSpriteEnd);

    // フェード用の黒画像
    Sprite2D::SPRITE_STATE ssFade{ WND_W, WND_H, WND_W, WND_H, WND_W, WND_H };
    m_pSpriteFade->Init(_T("Data\\Image\\Setting\\Black.png"), ssFade);
    m_upFade->AttachSprite(m_pSpriteFade);
    m_upFade->SetPosition({ 0.0f, 0.0f, 0.0f });
    m_upFade->SetAlpha(0.0f);

    Sprite2D::SPRITE_STATE ssBack{ 320,80,320,80,320,80 };
    m_pSpriteSelectBack->Init(_T("Data\\Image\\Setting\\SelectBack.png"), ssBack);
    m_upSelectBack->AttachSprite(m_pSpriteSelectBack);
    m_upSelectBack->SetPosition(m_StartPos);

    m_pSpriteSelectFrame->Init(_T("Data\\Image\\Setting\\SelectFrame.png"), ssBack);
    m_upSelectFrame->AttachSprite(m_pSpriteSelectFrame);
    m_upSelectFrame->SetPosition(m_StartPos);

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

    // --- 選択演出の描画 ---
    D3DXVECTOR3 currentSelectPos = (m_Select == SelectMenu::Start) ? m_StartPos : m_EndPos;

    D3DXVECTOR3 backPos = currentSelectPos;
    backPos.x -= 100.0f; // 背景を左にずらして、文字を背景の中央に合わせる
    backPos.y -= 15.0f; // 背景を少し上にずらして上下の中央を合わせる

    // 青い背景
    m_upSelectBack->SetPosition(backPos);
    m_upSelectBack->Draw();

    // 選択枠（フレーム）も同じ位置に
    m_upSelectFrame->SetPosition(backPos);
    m_upSelectFrame->Draw();

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
        m_StartPos = { 550.f, 550.f, 0.f };
        m_EndPos = { 550.f, 650.f, 0.f };
    }
    else
    {
        m_StartPos = { 550.f, 550.f, 0.f };
        m_EndPos = { 550.f, 650.f, 0.f };
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
