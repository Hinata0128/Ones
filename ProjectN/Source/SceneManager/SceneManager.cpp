#include "SceneManager.h"
#include "Sound/SoundManager.h"

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

//シーン作成.
void SceneManager::MakeScene(List Scene)
{
	switch (Scene)
	{
	//case SceneManager::Title:
	//	m_pScene = std::make_unique<Title>();
	//	break;
	case SceneManager::Main:
		m_pScene = std::make_unique<GameMain>();
		break;
	//case SceneManager::GameOver:
	//	m_pScene = std::make_unique<GameOver>();
	//	break;
	//case SceneManager::Ending:
	//	m_pScene = std::make_unique<Ending>();
	//	break;
	default:
		break;
	}
}
