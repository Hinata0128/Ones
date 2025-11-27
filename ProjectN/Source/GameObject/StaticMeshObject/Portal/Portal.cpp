#include "Portal.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"

Portal::Portal()
	: StaticMeshObject	()
	, m_PortalIncrease	(0)
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Portal));

	//ポータルの位置を変更.
	D3DXVECTOR3 Pos		= { 0.0f, 0.0f, 5.0f };
	//ポータルのサイズ変更.
	D3DXVECTOR3 Scale	= { 1.5f, 1.5f, 1.5f };

	SetPosition(Pos);
	SetScale(D3DXVECTOR3(Scale));

	Init();
}

Portal::~Portal()
{
}

void Portal::Update()
{
	//============================================================================================================
	// ポータルの取得ゲージの増加をImGuiを使用して確認します.
	// 100%になった時に確認のため一回EndingであるWin画面に遷移させるようにする.
	//============================================================================================================


#ifdef _DEBUG

	ImGui::Begin(JAPANESE("Portal : 増加量"));
	ImGui::End();
#endif


	StaticMeshObject::Update();
}

void Portal::Draw()
{
	StaticMeshObject::Draw();
}

void Portal::Init()
{
}
