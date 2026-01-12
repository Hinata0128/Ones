#include "Com.h"

#include "GameObject//StaticMeshObject//01_Portal//Portal.h"

#include "GameObject/SkinMeshObject/Character/EnemyBase/00_Boss/Boss.h"

#include "..//..//..//..//..//..//System/00_Manager/03_ImGuiManager/ImGuiManager.h"

#include "System/02_Singleton/Timer/Timer.h"

#include "..//00_BossContext/BossContext.h"

Com::Com(
	Boss* pOwner,
	std::shared_ptr<Portal> pPortal)
	: m_pPortal	(pPortal)
	, m_pOwner(pOwner)

	, m_Difficulty(BossDifficulty::Easy)

	, m_MoveSpeed(1.5f)

	, m_ShotInterval(0.0f)
	, m_DefenseRadius(0.0f)
{
	ApplyDifficultyParam();
}

Com::~Com()
{
}

void Com::Update()
{
	DecideAction();
}

void Com::DrawDebugImGui()
{
#ifdef _DEBUG
	ImGui::Begin(JAPANESE("Com: 内部ステータス"));

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

void Com::DecideDifficultyByRound(int raund)
{
	int Raund = rand() % 100;

	if (raund == 1)
	{
		//7割りEasyモード.
		if (Raund < 70)
		{
			SetDifficulty(BossDifficulty::Easy);
		}
		//残りHardモード.
		else
		{
			SetDifficulty(BossDifficulty::Hard);
		}
	}
	else if (raund == 2)
	{
		//8割りHardモード.
		if (Raund < 80)
		{
			SetDifficulty(BossDifficulty::Hard);
		}
		//残りEasyモード.
		else
		{
			SetDifficulty(BossDifficulty::Easy);
		}
	}
	//最終ラウンドは、固定.
	else if (raund == 3)
	{
		SetDifficulty(BossDifficulty::Final);
	}
}

//難易度用Set関数.
void Com::SetDifficulty(BossDifficulty diff)
{
	//引数代入.
	m_Difficulty = diff;
	ApplyDifficultyParam();
}

void Com::ApplyDifficultyParam()
{
	if (m_Difficulty == BossDifficulty::Easy)
	{
		m_MoveSpeed = 1.2f;
		m_ShotInterval = 5.0f;
		m_DefenseRadius = 6.0f;
	}
	else if (m_Difficulty == BossDifficulty::Hard)
	{
		m_MoveSpeed = 1.8f;
		m_ShotInterval = 0.8f;
		m_DefenseRadius = 8.0f;
	}
	else if (m_Difficulty == BossDifficulty::Final)
	{
		m_MoveSpeed = 2.4f;
		m_ShotInterval = 0.4f;
		m_DefenseRadius = 10.0f;
	}

	//Boss側に発射間隔を反映.
	m_pOwner->SetShotInterval(m_ShotInterval);

}


//1. 優先度1の処理を書く関数.
void Com::DecideAction()
{
	//敵の位置を取得する.
	D3DXVECTOR3 BossPos = m_pOwner->GetPosition();
	//ポータルの位置を取得する.
	D3DXVECTOR3 PortalPos = m_pPortal->GetPosition();

	//ポータルの位置 - 敵の位置の取得.
	D3DXVECTOR3 Diff = PortalPos - BossPos;

	//ポータルから敵への長さ(距離を入手する).
	float Dist = D3DXVec3Length(&Diff);

	//ローカル変数(ポータルを触る距離).
	float PortalGet = 5.0f;

	Portal::PortalPriority state = m_pPortal->GetPortalState();

	// 優先度1：未取得 → 取りに行く
	if (state == Portal::PortalPriority::None)
	{
		MoveToPortal();
		return;
	}

	// 優先度2：自分が所持 → 防衛
	if (state == Portal::PortalPriority::Enemy)
	{
		Defense();
		return;
	}

	// 優先度3：プレイヤーが所持 → 奪いに行く
	if (state == Portal::PortalPriority::Player)
	{
		if (Dist > PortalGet)
		{
			MoveToPortal();
		}
		PlayerAttack();
	}
}

void Com::MoveToPortal()
{
	float dt = Timer::GetInstance().DeltaTime();

	D3DXVECTOR3 bossPos = m_pOwner->GetPosition();
	D3DXVECTOR3 portalPos = m_pPortal->GetPosition();
	D3DXVECTOR3 dir = portalPos - bossPos;

	float len = D3DXVec3Length(&dir);
	BossContext ctx(m_pOwner);

	if (len > 0.01f)
	{
		D3DXVec3Normalize(&dir, &dir);
		m_pOwner->AddPosition(dir * m_MoveSpeed * dt);

		float angle = std::atan2f(-dir.x, -dir.z);
		m_pOwner->SetRotationY(angle);

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
}

void Com::PlayerAttack()
{
	m_pOwner->RequestShot();
}

void Com::Defense()
{
	float dt = Timer::GetInstance().DeltaTime();

	D3DXVECTOR3 bossPos = m_pOwner->GetPosition();
	D3DXVECTOR3 portalPos = m_pPortal->GetPosition();
	D3DXVECTOR3 playerPos = m_pOwner->GetPlayerPos();

	BossContext ctx(m_pOwner);

	// ① プレイヤーを見る
	D3DXVECTOR3 look = playerPos - bossPos;
	look.y = 0.0f;
	D3DXVec3Normalize(&look, &look);
	float angle = std::atan2f(-look.x, -look.z);
	m_pOwner->SetRotationY(angle);

	// ② ポータル中心基準の方向
	D3DXVECTOR3 toBoss = bossPos - portalPos;
	toBoss.y = 0.0f;

	float dist = D3DXVec3Length(&toBoss);
	if (dist < 0.01f)
		return;

	D3DXVec3Normalize(&toBoss, &toBoss);

	// ③ 円運動用の接線ベクトル
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 tangent;
	D3DXVec3Cross(&tangent, &up, &toBoss);

	// ④ 円運動
	D3DXVECTOR3 velocity = tangent * m_MoveSpeed;

	// ⑤ 半径制御
	if (dist > m_DefenseRadius)
	{
		velocity -= toBoss * m_MoveSpeed;
	}

	m_pOwner->AddPosition(velocity * dt);

	// ⑥ 歩きアニメ
	const int WALK_ANIM = 2;
	if (ctx.AnimNo != WALK_ANIM)
	{
		ctx.AnimNo = WALK_ANIM;
		ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
	}

	// ⑦ 攻撃
	m_pOwner->RequestShot();
}
