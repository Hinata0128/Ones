#include "CSceneBase.h"

CSceneBase::CSceneBase()
	: m_pMesh	(nullptr)
	, m_pDx11	(DirectX11::GetInstance())
	, m_pDx9	(DirectX9::GetInstance())
	, m_hWnd	(nullptr)
	, m_Score	(0)
	, m_Count	(0)
{
}

void CSceneBase::PreDraw()
{
	Renderer::GetInstance().SetDrawParameters(
		m_mView,
		m_mProj,
		m_Light,
		m_Camera);
}
