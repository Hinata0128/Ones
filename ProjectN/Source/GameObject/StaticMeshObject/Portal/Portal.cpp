#include "Portal.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"
#include "../..//..//System/02_Singleton/Timer/Timer.h"

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

	static float elapsedTime = 0.0f;
	float delta = Timer::GetInstance().DeltaTime(); // フレームの経過時間
	elapsedTime += delta;

	if (elapsedTime >= 1.0f)   // 1秒経過したら
	{
		elapsedTime = 0.0f;
		m_PortalIncrease += 5; // 増加量（好きに調整OK）

		if (m_PortalIncrease > 100)
			m_PortalIncrease = 100;
	}

#ifdef _DEBUG

	ImGui::Begin(JAPANESE("Portal : 増加量"));

	// スライダー（手動調整も可能）
	ImGui::SliderInt("Portal Gauge (%)", &m_PortalIncrease, 0, 100);

	ImGui::Text("Current Gauge : %d %%", m_PortalIncrease);

	if (m_PortalIncrease >= 100)
	{
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Portal FULL!");
		// TODO: シーン遷移を書く
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
