#include "FirstRound.h"

#include "SceneManager/SceneManager.h"
#include "System/02_Singleton/Timer/Timer.h"


FirstRound::FirstRound()
	: SceneBase			()
	//, m_pFirstRoundImg	( nullptr)
	//, m_pFirstRound		( std::make_shared<UIObject>() )

	, m_pSpriteTitle(std::make_shared<Sprite2D>())
	, m_pTitleObj(std::make_shared<UIObject>())
	
	, m_Timer	(0.0f)

{
}

FirstRound::~FirstRound()
{
}

void FirstRound::Initialize()
{
}

void FirstRound::Create()
{
	//タイトル構造体.
	Sprite2D::SPRITE_STATE SSTitle =
	{ 1280.f, 720.f, 896.f, 560.f, 896.f , 560.f };

	//タイトルスプライト読み込み.
	m_pSpriteTitle->Init(_T("Data\\Texture\\FirstRoundImg.png"), SSTitle);


	m_pTitleObj->AttachSprite(m_pSpriteTitle);

	// 位置などを必要ならセット
	m_pTitleObj->SetPosition(D3DXVECTOR3(0, 0, 0));
}

void FirstRound::Update()
{
	//タイマーの動作関数.
	//Mainクラスで書くと、弾の発射が機能しなくなったのでここで呼ぶ.
	Timer::GetInstance().Update();

	float deltaTime = Timer::GetInstance().DeltaTime();

	m_Timer += deltaTime;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Main);
	}

	if (m_Timer >= 0.5f)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Main);
		return;
	}

}

void FirstRound::Draw()
{
	m_pTitleObj->Draw();
}
