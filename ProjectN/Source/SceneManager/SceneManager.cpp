#include "SceneManager.h"
#include "Sound/SoundManager.h"

#include "..//System/00_Manager/03_ImGuiManager/ImGuiManager.h"

SceneManager::SceneManager()
	: m_pScene	( nullptr )
	, m_hWnd	()
	, m_pDx11	()
	, m_pDx9	()
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
	//==============================
	//  ImGui デバッグ表示：ポイント
	//==============================
	ImGui::Begin("Game Score");

	ImGui::Text("Player Score : %d", playerScore);
	ImGui::Text("Enemy  Score : %d", enemyScore);

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

void SceneManager::AddPlayerScore()
{
	playerScore++;

	if (playerScore >= 2)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Win);
		return;
	}

	ResetRound();
}

void SceneManager::AddEnemyScore()
{
	enemyScore++;

	if (enemyScore >= 2)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Lose);
		return;
	}

	ResetRound();
}

void SceneManager::ResetRound()
{
	auto portal = SceneManager::GetInstance()->GetPortal();
	if (portal)
	{
		portal->Init();
	}
}
//シーン作成.
void SceneManager::MakeScene(List Scene)
{
	switch (Scene)
	{
	case SceneManager::OP:
		m_pScene = std::make_unique<Title>();
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
