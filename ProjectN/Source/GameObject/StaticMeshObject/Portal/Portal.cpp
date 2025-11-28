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

	//経過時間を設定.
	static float ElapsedTime = 0.0f;
	//deltaTimeの取得.
	float deltaTime = Timer::GetInstance().DeltaTime();
	//経過時間に今の時間をたす.
	ElapsedTime += deltaTime;

	//もし一秒経過したら.
	if (ElapsedTime >= 1.0f)
	{
		ElapsedTime = 0.0f;
		m_PortalIncrease += 1;	//時間経過で取得可能な数値.

		if (m_PortalIncrease > 100)
		{
			m_PortalIncrease = 100;
		}
	}

#ifdef _DEBUG

	ImGui::Begin(JAPANESE("Portal : 増加量"));

	//スライダー(手動で触れるようにしている).
	ImGui::SliderInt(JAPANESE("Portalの取得パーセント(%)"), &m_PortalIncrease, 0, 100);
	//今進んでいるパーセンテージ.
	ImGui::Text(JAPANESE("今のポータルのパーセント : %d"), m_PortalIncrease);

	if (m_PortalIncrease >= 100)
	{
		//ポータルが100%になった時に表示してくれるもの.
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Portal Full");
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
