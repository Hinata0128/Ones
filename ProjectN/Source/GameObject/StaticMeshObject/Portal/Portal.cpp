#include "Portal.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"
#include "System/02_Singleton/Timer/Timer.h"
#include "GameObject/SkinMeshObject/Character/Player/Player.h"
#include "GameObject/SkinMeshObject/Character/EnemyBase/EnemyNomal/EnemyNomal.h"

#include "SceneManager/SceneManager.h"

Portal::Portal()
	: StaticMeshObject	()
	, m_PortalIncreaseF(0.0f)
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

	//Playerがポータルの範囲に入った時にImGuiのゲージを増加させる.
#if 1
	//今deltaTimeでポータルのパーセント取得をしている.
	//今からしたいのはPlayerの距離でポータルのパーセントを進めれる設計にする.
	if (auto player = m_pPlayer.lock())
	{
		D3DXVECTOR3 diff = player->GetPosition() - GetPosition();
		float distance = D3DXVec3Length(&diff);

		constexpr float PORTAL_DISTANCE = 20.0f;
		float deltaTime = Timer::GetInstance().DeltaTime();

		if (distance <= PORTAL_DISTANCE)
		{
			m_PortalIncreaseF += deltaTime;
			if (m_PortalIncreaseF > 100.0f) m_PortalIncreaseF = 100.0f;

			m_PortalIncrease = static_cast<int>(m_PortalIncreaseF);
		}
	}
#endif

	//敵がポータルの範囲に入った時ImGuiのゲージを増加させる.

	if (auto enemy = m_pEnemy.lock())
	{
		D3DXVECTOR3 Diff = enemy->GetPosition() - GetPosition();
		float Distance = D3DXVec3Length(&Diff);

		constexpr float Portal_Distance  = 20.0f;
		float deltaTime = Timer::GetInstance().DeltaTime();

		if (Distance <= Portal_Distance)
		{
			m_PortalIncreaseF += deltaTime;
			if (m_PortalIncreaseF > 100.0f)
			{
				m_PortalIncreaseF = 100.0f;

				SceneManager::GetInstance()->LoadScene(SceneManager::Win);
				m_PortalIncrease = static_cast<int>(m_PortalIncreaseF);
			}
		}
	}
	
	//時間経過での増加.
#if 0
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
#endif

#ifdef _DEBUG

	//表示するブロック.
	ImGui::Begin(JAPANESE("Portal : 増加量"));

	//スライダー(手動で触れるようにしている).
	ImGui::SliderFloat(JAPANESE("Portalの取得パーセント(%)"), &m_PortalIncreaseF, 0.0f, 100.0f);
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
