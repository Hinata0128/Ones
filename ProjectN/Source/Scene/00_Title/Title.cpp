#include "Title.h"

#include "SceneManager/SceneManager.h"
#include "Sound/SoundManager.h"

#include "GameObject/UIObject/UIObject.h"

#include "Sprite2D/Sprite2D.h"

#include "DirectX/DirectX11.h"

Title::Title()
	: SceneBase()

    , m_Select(SelectMenu::Start)

    , m_pSpriteTitle(std::make_shared<Sprite2D>())
    , m_upTitle(std::make_shared<UIObject>())

    , m_pSpriteStart(std::make_shared<Sprite2D>())
    , m_upStart(std::make_shared<UIObject>())

    , m_pSpriteEnd(std::make_shared<Sprite2D>())
    , m_upEnd(std::make_shared<UIObject>())
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
    //タイトル画像のサイズ(幅・高さ).
    const float WND_W = 1280.0f;
    const float WND_H = 720.0f;

    //タイトル画像サイズの設定.
    Sprite2D::SPRITE_STATE SSTitle =
    {
        WND_W,WND_H,
        WND_W,WND_H,
        WND_W,WND_H,
    };

    //スタート画像のサイズ(幅・高さ)
    const float Start_W = 135.0f;
    const float Start_H = 45.0f;

    //スタート・エンドの画像サイズの設定.
    Sprite2D::SPRITE_STATE SSStart =
    {
        Start_W,Start_H,
        Start_W,Start_H,
        Start_W,Start_H
    };

    const float End_W = 84.0f;
    const float End_H = 45.0f;

    //エンドの画像サイズの設定.
    Sprite2D::SPRITE_STATE SSEnd =
    {
        End_W,End_H,
        End_W,End_H,
        End_W,End_H
    };

    //タイトル画像の読み込み.
    m_pSpriteTitle->Init(_T("Data\\Image\\Setting\\Surface.png"), SSTitle);
    //画像設定.
    m_upTitle->AttachSprite(m_pSpriteTitle);
    //画像表示位置.
    m_upTitle->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.f));

    //スタート画像の読み込み.
    m_pSpriteStart->Init(_T("Data\\Image\\Setting\\S_Start.png"), SSStart);
    //画像設定.
    m_upStart->AttachSprite(m_pSpriteStart);
    //画像表示位置.
    m_upStart->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.f));

    //エンド画像の読み込み.
    m_pSpriteEnd->Init(_T("Data\\Image\\Setting\\S_End.png"), SSEnd);
    //画像設定.
    m_upEnd->AttachSprite(m_pSpriteEnd);
    //画像表示位置.
    m_upEnd->SetPosition(D3DXVECTOR3(0.0f, 50.0f, 0.f));

}

void Title::Update()
{
    // ↓キー
    if (GetAsyncKeyState(VK_DOWN) & 0x0001)
    {
        m_Select = SelectMenu::End;
    }

    // ↑キー
    if (GetAsyncKeyState(VK_UP) & 0x0001)
    {
        m_Select = SelectMenu::Start;
    }

    // 決定
    if (GetAsyncKeyState(VK_RETURN) & 0x0001)
    {
        Decide();
    }
}

void Title::Draw()
{
    DirectX11::GetInstance()->SetDepth(false);

    m_upTitle->Draw();

    if (m_Select == SelectMenu::Start)
    {
        m_upStart->SetPosition(D3DXVECTOR3(20.0f, 0.0f, 0.f));
        m_upEnd->SetPosition(D3DXVECTOR3(0.0f, 50.0f, 0.f));
    }
    else
    {
        m_upStart->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.f));
        m_upEnd->SetPosition(D3DXVECTOR3(20.0f, 50.0f, 0.f));
    }

    m_upStart->Draw();
    m_upEnd->Draw();

    DirectX11::GetInstance()->SetDepth(true);

}

void Title::Decide()
{
    switch (m_Select)
    {
    case SelectMenu::Start:
        if (SceneManager::GetInstance()->IsGameFinished())
        {
            SceneManager::GetInstance()->ResetScore();
        }
        SceneManager::GetInstance()->LoadScene(SceneManager::Main);
        break;

    case SelectMenu::End:
        PostQuitMessage(0); // ゲーム終了
        break;
    }
}
