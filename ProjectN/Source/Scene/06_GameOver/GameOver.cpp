#include "GameOver.h"

#include "SceneManager//SceneManager.h"
#include "GameObject//03_UIObject//UIObject.h"
#include "Sprite2D//Sprite2D.h"

#include "DirectX//DirectX11.h"

GameOver::GameOver()
	: SceneBase			()

	, m_pSpriteBack		( std::make_shared<Sprite2D>() )
	, m_upBack			( std::make_shared<UIObject>() )

	, m_pSpriteDefeat	( std::make_shared<Sprite2D>() )
	, m_upDefeat		( std::make_shared<UIObject>() )
{
}

GameOver::~GameOver()
{
}

void GameOver::Initialize()
{
}

void GameOver::Create()
{
	//背景画像サイズのローカル変数.
	const float WND_W = 1280.0f;
	const float WMD_H = 720.0f;

	//Lose画像のサイズのローカル変数.
	const float Defeat_W = 430.0f;
	const float Defeat_H = 210.0f;

	//背景構造体.
	Sprite2D::SPRITE_STATE SSBack =
	{
		WND_W, WMD_H, WND_W, WMD_H, WND_W, WMD_H
	};
	//背景画面の読み込み.
	m_pSpriteBack->Init(_T("Data\\Image\\Setting\\Lose.png"), SSBack);
	//画像の設定.
	m_upBack->AttachSprite(m_pSpriteBack);
	//表示位置.
	m_upBack->SetPosition(0.0f, 0.0f, 0.0f);

	//Lose構造体.
	Sprite2D::SPRITE_STATE SSLose =
	{
		Defeat_W, Defeat_H, Defeat_W, Defeat_H, Defeat_W, Defeat_H
	};
	//Lose画像の読み込み.
	m_pSpriteDefeat->Init(_T("Data\\Image\\Setting\\Defeat.png"), SSLose);
	//画像の設定.
	m_upDefeat->AttachSprite(m_pSpriteDefeat);
	//表示位置.
	m_upDefeat->SetPosition(150.0f, 100.0f, 0.0f);
}

void GameOver::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::OP);
	}
}

void GameOver::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	m_upBack->Draw();
	m_upDefeat->Draw();
	DirectX11::GetInstance()->SetDepth(true);
}
