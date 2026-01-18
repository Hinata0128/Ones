#include "Title.h"

#include "SceneManager/SceneManager.h"
#include "Sound/SoundManager.h"

#include "GameObject/UIObject/UIObject.h"

#include "Sprite2D/Sprite2D.h"

#include "DirectX/DirectX11.h"

Title::Title()
	: SceneBase()

    , m_pSpriteTitle(std::make_shared<Sprite2D>())
    , m_upTitle(std::make_shared<UIObject>())
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
    const float WND_W = 1280.0f;
    const float WND_H = 720.0f;

    //タイトル画像の設定.
    Sprite2D::SPRITE_STATE SSImage =
    {
        WND_W,WND_H,
        WND_W,WND_H,
        WND_W,WND_H,
    };

    //タイトル画像の読み込み.
    m_pSpriteTitle->Init(_T("Data\\Image\\Setting\\Surface.png"), SSImage);
    //画像設定.
    m_upTitle->AttachSprite(m_pSpriteTitle);
    //画像表示位置.
    m_upTitle->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.f));
}

void Title::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
        //2点先取チェック：勝敗がついていたら完全リセット
        if (SceneManager::GetInstance()->IsGameFinished())
        {
            SceneManager::GetInstance()->ResetScore();
        }

        //次のゲームへ.
        SceneManager::GetInstance()->LoadScene(SceneManager::Main);
    }
}

void Title::Draw()
{
    DirectX11::GetInstance()->SetDepth(false);
    m_upTitle->Draw();
    DirectX11::GetInstance()->SetDepth(true);
}