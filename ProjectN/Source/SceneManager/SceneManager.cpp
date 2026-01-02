#include "SceneManager.h"
#include "Sound/SoundManager.h"

#include "..//System/00_Manager/03_ImGuiManager/ImGuiManager.h"

SceneManager::SceneManager()
	: m_pScene			( nullptr )
	, m_hWnd			()
	, m_pDx11			()
	, m_pDx9			()

	, m_pPortal			( nullptr )
	, m_PlayerPoint		( 0 )
	, m_EnemyNomalPoint	( 0 )
{

}

SceneManager::~SceneManager()
{
}

HRESULT SceneManager::Create(HWND hWnd)
{
	m_hWnd = hWnd;
	LoadScene(List::Main);
	return S_OK;
}

void SceneManager::Update()
{
	m_pScene->Update();

#ifdef _DEBUG
	//========================================================================================================
	//	ImGuiを使用して今PlayerとEnemyNomalが取得しているポイントを表示しています.
	//========================================================================================================
	ImGui::Begin("Game Score");

	ImGui::Text("Player Score : %d", m_PlayerPoint);
	ImGui::Text("Enemy  Score : %d", m_EnemyNomalPoint);

	// ポータルが存在していれば、進行状況も確認可能
	if (m_pPortal)
	{
		ImGui::Separator();
		ImGui::Text("Portal Progress : %d %%", m_pPortal->GetPortalPercent());
	}

	ImGui::End();
#endif
}

void SceneManager::Draw()
{
	m_pScene->Draw();
}

void SceneManager::LoadScene(List Scene)
{
	m_pScene.reset();

	//シーン作成.
	MakeScene(Scene);
	m_pScene->Create();
}

HWND SceneManager::GetHWND() const
{
	return m_hWnd;
}

void SceneManager::SetPortal(Portal* portal)
{
	m_pPortal = portal;
}

Portal* SceneManager::GetPortal() const
{
	return m_pPortal;
}

//Playerのポータル取得ポイント関数.
void SceneManager::AddPlayerScore()
{
	m_PlayerPoint++;

	if (m_PlayerPoint >= 2)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Win);
		return;
	}

	ResetRound();
}

//EnemyNomalのポータル取得ポイント関数.
void SceneManager::AddEnemyScore()
{
	m_EnemyNomalPoint++;

	if (m_EnemyNomalPoint >= 2)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Lose);
		return;
	}

	ResetRound();
}

int SceneManager::GetPlayerScore() const
{
	return m_PlayerPoint;
}

int SceneManager::GetEnemyScore() const
{
	return m_EnemyNomalPoint;
}

void SceneManager::ResetRound()
{
	auto portal = SceneManager::GetInstance()->GetPortal();
	if (portal)
	{
		portal->Init();
	}
}

bool SceneManager::IsGameFinished() const
{
	return (m_PlayerPoint >= 2 || m_EnemyNomalPoint >= 2);

}
void SceneManager::ResetScore()
{
	m_PlayerPoint = 0;
	m_EnemyNomalPoint = 0;
}

//シーン作成.
void SceneManager::MakeScene(List Scene)
{
	switch (Scene)
	{
	case SceneManager::OP:
		m_pScene = std::make_unique<Title>();
		break;
		case SceneManager::First:
		m_pScene = std::make_unique<FirstRound>();
		break;
	case SceneManager::Main:
		m_pScene = std::make_unique<GameMain>();
		break;
	case SceneManager::Lose:
		m_pScene = std::make_unique<GameOver>();
		break;
	case SceneManager::Win:
		m_pScene = std::make_unique<Ending>();
		break;
	default:
		break;
	}
}
