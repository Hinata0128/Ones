#include "GameOver.h"

#include "SceneManager//SceneManager.h"
#include "GameObject//03_UIObject//UIObject.h"
#include "Sprite2D//Sprite2D.h"

#include "DirectX//DirectX11.h"

GameOver::GameOver()
	: SceneBase			()

	, m_Select			( SelectMenu::Continue )
	, m_State			( LoseState::Select )

	, m_pSpriteBack		( std::make_shared<Sprite2D>() )
	, m_upBack			( std::make_shared<UIObject>() )

	, m_pSpriteDefeat	( std::make_shared<Sprite2D>() )
	, m_upDefeat		( std::make_shared<UIObject>() )

	, m_pSpriteContinue	( std::make_shared<Sprite2D>() )
	, m_upContinue		( std::make_shared<UIObject>() )

	, m_pSpriteEnd		( std::make_shared<Sprite2D>() )
	, m_upEnd			( std::make_shared<UIObject>() )
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

	//Continue画像のサイズのローカル変数.
	const float Continue_W = 194.0f;
	const float Continue_H = 45.0f;

	//End画像のサイズのローカル変数.
	const float End_W = 84.0f;
	const float End_H = 45.0f;

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

	//Continue構造体.
	Sprite2D::SPRITE_STATE SSContinue =
	{
		Continue_W, Continue_H, Continue_W, Continue_H, Continue_W, Continue_H
	};
	//Continue画像の読み込み.
	m_pSpriteContinue->Init(_T("Data\\Image\\Setting\\S_Continue.png"), SSContinue);
	//画像の設定.
	m_upContinue->AttachSprite(m_pSpriteContinue);
	//表示位置.
	m_upContinue->SetPosition(880.0f, 520.0f, 0.0f);

	//End構造体.
	Sprite2D::SPRITE_STATE SSEnd =
	{
		End_W, End_H, End_W, End_H, End_W, End_H,
	};
	//End画像の読み込み.
	m_pSpriteEnd->Init(_T("Data\\Image\\Setting\\S_End.png"), SSEnd);
	//画像の設定.
	m_upEnd->AttachSprite(m_pSpriteEnd);
	//表示位置.
	m_upEnd->SetPosition(920.0f, 620.0f, 0.0f);
}

void GameOver::Update()
{
	//最終的には選択できるように表示する.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::First);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::OP);
	}
}

void GameOver::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	m_upBack->Draw();
	m_upDefeat->Draw();
	m_upContinue->Draw();
	m_upEnd->Draw();
	DirectX11::GetInstance()->SetDepth(true);
}
