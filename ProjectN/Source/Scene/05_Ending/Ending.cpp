#include "Ending.h"

#include "SceneManager//SceneManager.h"
#include "GameObject//03_UIObject//UIObject.h"
#include "Sprite2D//Sprite2D.h"

#include "DirectX//DirectX11.h"


Ending::Ending()
	: SceneBase			()

	, m_pSpriteBack		( std::make_shared<Sprite2D>() )
	, m_upBack			( std::make_shared<UIObject>() )

	, m_pSpriteVictory	( std::make_shared<Sprite2D>() )
	, m_upVictory		( std::make_shared<UIObject>() )
{
}

Ending::~Ending()
{
}

void Ending::Initialize()
{
}

void Ending::Create()
{
	//背景画像サイズのローカル変数.
	const float WND_W = 1280.0f;
	const float WMD_H = 720.0f;

	//Win画像のサイズのローカル変数.
	const float Win_W = 430.0f;
	const float Win_H = 210.0f;

	//背景構造体.
	Sprite2D::SPRITE_STATE SSBack =
	{
		WND_W, WND_H, WND_W, WND_H, WND_W, WND_H
	};
	//背景画像の読み込み.
	m_pSpriteBack->Init(_T("Data\\Image\\Setting\\Win.png"), SSBack);
	//画像の設定.
	m_upBack->AttachSprite(m_pSpriteBack);
	//表示位置.
	m_upBack->SetPosition(0.0f, 0.0f, 0.0f);

	//Win構造体
	Sprite2D::SPRITE_STATE SSWin =
	{
		Win_W, Win_H, Win_W, Win_H, Win_W, Win_H
	};
	//Win画像の読み込み.
	m_pSpriteVictory->Init(_T("Data\\Image\\Setting\\Victory.png"), SSWin);
	//画像の設定.
	m_upVictory->AttachSprite(m_pSpriteVictory);
	//表示位置.
	m_upVictory->SetPosition(150.0f, 100.0f, 0.0f);

}

void Ending::Update()
{
	//最終的には選択できるように表示する.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::OP);
	}
}

void Ending::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	m_upBack->Draw();
	m_upVictory->Draw();
	DirectX11::GetInstance()->SetDepth(true);
}
