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

	//個人的に敵の攻撃で使用しているコードをもとに作成するとPlayerの位置系統が実装できるのでパーセントの実装ができると思う.

#ifdef _DEBUG

	ImGui::Begin(JAPANESE("Portal : 増加量"));

	// スライダーで % を操作
	ImGui::SliderInt("Portal Gauge (%)", &m_PortalIncrease, 0, 100);

	// 表示確認用
	ImGui::Text("Current Gauge : %d %%", m_PortalIncrease);

	// 100% に到達したらイベントなどを出せる
	if (m_PortalIncrease >= 100)
	{
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Portal FULL!");
		//ここにシーン遷移をかく.
	}

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
