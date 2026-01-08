#include "BossAI.h"

#include "GameObject//StaticMeshObject//01_Portal//Portal.h"

#include "GameObject/SkinMeshObject/Character/EnemyBase/00_Boss/Boss.h"

#include "..//..//..//..//..//..//System/00_Manager/03_ImGuiManager/ImGuiManager.h"

#include "System/02_Singleton/Timer/Timer.h"

#include "..//00_BossContext/BossContext.h"

BossAI::BossAI(
	Boss* pOwner,
	std::shared_ptr<Portal> pPortal)
	: m_pPortal	(pPortal)
	, m_pOwner(pOwner)

	, m_MoveSpeed(1.5f)
{
}

BossAI::~BossAI()
{
}

void BossAI::Update()
{


	DecideAction();
}

void BossAI::DrawDebugImGui()
{
#ifdef _DEBUG
	ImGui::Begin(JAPANESE("BossAI: 内部ステータス"));

	// 1. ポータルの認識状態
	Portal::PortalPriority PortalState = m_pPortal->GetPortalState();
	const char* stateStr = "Unknown";
	ImVec4 color = ImVec4(1, 1, 1, 1); // デフォルト白

	if (PortalState == Portal::PortalPriority::None) {
		stateStr = "None";
		color = ImVec4(0, 1, 0, 1); // 緑
	}
	else if (PortalState == Portal::PortalPriority::Player) {
		stateStr = "Player (Attack!)";
		color = ImVec4(0, 0, 1, 1); // 青
	}
	else if (PortalState == Portal::PortalPriority::Enemy) {
		stateStr = "Enemy (Defense)";
		color = ImVec4(1, 0, 0, 1); // 赤
	}

	ImGui::Text(JAPANESE("AIが見ている状態: "));
	ImGui::SameLine();
	ImGui::TextColored(color, stateStr);

	// 2. 距離情報の表示
	D3DXVECTOR3 BossPos = m_pOwner->GetPosition();
	D3DXVECTOR3 PortalPos = m_pPortal->GetPosition();
	D3DXVECTOR3 Length = PortalPos - BossPos;
	float Pos_Lenght = D3DXVec3Length(&Length);

	ImGui::Text(JAPANESE("ポータルへの距離: %.2f"), Pos_Lenght);
	ImGui::Text(JAPANESE("占拠可能距離: 5.0f"));

	// 3. 実行中の行動推測（if文のどこを通っているか）
	if (PortalState == Portal::PortalPriority::None && Pos_Lenght > 5.0f) {
		ImGui::BulletText(JAPANESE("現在: MoveToPortal (移動中)"));
	}
	else if (PortalState == Portal::PortalPriority::Enemy) {
		ImGui::BulletText(JAPANESE("現在: Defense (防衛/AutoShot)"));
	}
	else if (PortalState == Portal::PortalPriority::Player) {
		ImGui::BulletText(JAPANESE("現在: PlayerAttack (攻撃)"));
	}
	else {
		ImGui::BulletText(JAPANESE("現在: 待機/占拠中"));
	}

	ImGui::End();
#endif
}

//1. 優先度1の処理を書く関数.
void BossAI::DecideAction()
{
	//敵の位置を取得する.
	D3DXVECTOR3 BossPos = m_pOwner->GetPosition();
	//ポータルの位置を取得する.
	D3DXVECTOR3 PortalPos = m_pPortal->GetPosition();

	//ポータルの位置 - 敵の位置の取得.
	D3DXVECTOR3 Length = PortalPos - BossPos;

	//ポータルから敵への長さ(距離を入手する).
	float Pos_Lenght = D3DXVec3Length(&Length);

	//ローカル変数(ポータルを触る距離).
	float PortalGet = 5.0f;

	if (Pos_Lenght > PortalGet)
	{
		MoveToPortl();
	}

	//ポータルの現在の情報を取得する.
	//ポータルクラスの中の列挙を呼んでいる.
	Portal::PortalPriority PortalState = m_pPortal->GetPortalState();

	//優先度1: 誰もポータルをとっていない、かつ、遠い場合はまず移動させる.
	if (PortalState == Portal::PortalPriority::None && Pos_Lenght > PortalGet)
	{
		//ポータルの取得用関数に入る.
		MoveToPortl();
	}
	//優先度2: 自分がポータルを取得していたら[防衛].
	else if (PortalState == Portal::PortalPriority::Enemy)
	{
		//DEBUG用で、体力の数値をいじっている.
		if (m_pOwner->GetEnemyHitPoint() > 90.0f)
		{
			//プレイヤー攻撃状態に入る.
			PlayerAttack();
		}
		else
		{
			//防衛状態に入る.
			Defense();
		}
	}
	//優先度2: プレイヤーが取得している、または、強奪が必要な場合.

	else if (PortalState == Portal::PortalPriority::Player)
	{
		PlayerAttack();
	}
}

void BossAI::MoveToPortl()
{
	//時間の取得.
	float deltaTime = Timer::GetInstance().DeltaTime();


	//ボスの位置を取得.
	D3DXVECTOR3 BossPos = m_pOwner->GetPosition();
	//ポータルの位置を取得している.
	D3DXVECTOR3 PortalPos = m_pPortal->GetPosition();

	//向きの計算.
	D3DXVECTOR3 BossToPortal = PortalPos - BossPos;

	//ポータルまでの距離を測る.
	float Dist = D3DXVec3Length(&BossToPortal);

	//ポータルの座標を取得.
	BossContext ctx(m_pOwner);

	//ポータルまで近づく.
	if (Dist > 5.0f)
	{
		//正規化.
		D3DXVECTOR3 dir;
		D3DXVec3Normalize(&dir, &BossToPortal);

			//移動量の計算.
		D3DXVECTOR3 Velocity = dir * m_MoveSpeed *  deltaTime;

		//ボスの位置の取得.
		m_pOwner->AddPosition(Velocity);

		//体の向きをポータルに合わせる.
		float Angle = std::atan2f(-dir.x, -dir.z);
		m_pOwner->SetRotationY(Angle);

		const int WALK_ANIM = 2;
		if (ctx.AnimNo != WALK_ANIM)
		{
			ctx.AnimNo = WALK_ANIM;
			ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
		}
	}
	else
	{
		const int IDLE_ANIM = 0; 
		if (ctx.AnimNo != IDLE_ANIM)
		{
			ctx.AnimNo = IDLE_ANIM;
			ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
		}
	}

	//ポータルの位置を指示する.
	m_pOwner->SetPortalPos(PortalPos);
}

void BossAI::PlayerAttack()
{
	//攻撃状態になったら遠距離攻撃を開始する.
	m_pOwner->AutoShot();
}

void BossAI::Defense()
{
	//守っているとき.
	MoveToPortl();
	//防衛中敵が近くにいてるときに攻撃をする.
	m_pOwner->AutoShot();
}
