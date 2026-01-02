#include "GameOver.h"
#include "Sound/SoundManager.h"

GameOver::GameOver()
	: SceneBase()
	, m_pHpBarSprite(std::make_shared<Sprite2D>())
	, m_pHpBar(std::make_shared<UIObject>())

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
	//タイトル構造体.
	Sprite2D::SPRITE_STATE SSTitle =
	{ 1280.f, 720.f, 896.f, 560.f, 896.f , 560.f };

	//タイトルスプライト読み込み.
	m_pHpBarSprite->Init(_T("Data\\Texture\\FirstRoundImg.png"), SSTitle);

	m_pHpBar->AttachSprite(m_pHpBarSprite);
	// 位置などを必要ならセット
	m_pHpBar->SetPosition(D3DXVECTOR3(0, 0, 0));
}

void GameOver::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Main);
	}
}

void GameOver::Draw()
{
	m_pHpBar->Draw();
}
