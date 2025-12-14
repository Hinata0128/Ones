#include "AIController.h"

#include "..//Boss.h"
#include "..//01_BossStateBase/BossStateBase.h"

#include "..//01_BossStateBase/00_BossIdol/BossIdol.h"
#include "..//01_BossStateBase/01_BossMove/BossMove.h"

AIController::AIController(Boss* owner)
	: m_pOwner				(owner)
	, m_CurrentCoreState	(AICoreState::none)
{
}

AIController::~AIController()
{
}

void AIController::Update()
{
	//優先順位に基づき、次にとるべきアクションの意図を決定する.
	AICoreState nextState = DecideNextAction();

	//決定した意図に基づき、BossのStateを切り替える.
	ExecuteStateTransition(nextState);
}

AIController::AICoreState AIController::DecideNextAction()
{
	//死亡判定.
	if (m_pOwner->GetEnemyHitPoint() <= 0.0f)
	{
		return AICoreState::Dead;
	}

	//退避判定(生存のため優先).
	if (LowHealth() && PlayerNearby(20.0f))
	{
		return AICoreState::Save;
	}

	//ポータルの取得判定(最優先).
	if (HasUncapturedPortal())
	{
		return AICoreState::PortalGet;
	}

	//Playerが当すぎる場合は動作判定.
	if (!PlayerNearby(80.0f))
	{
		return AICoreState::Move;
	}

	if (PlayerNearby(30.0f))
	{
		return AICoreState::Move;
	}

	//デフォルト判定.
	return AICoreState::PlayerAttack;
}

void AIController::ExecuteStateTransition(AICoreState NextState)
{
	if (m_CurrentCoreState == NextState)
	{
		return;
	}

	BossStateBase* TargetState = nullptr;

	switch (NextState)
	{
	case AIController::AICoreState::Idol:
		TargetState = m_pOwner->m_pIdol.get();
		break;
	case AIController::AICoreState::Move:
		TargetState = m_pOwner->m_pMove.get();
		break;
	case AIController::AICoreState::Dead:
		TargetState = m_pOwner->m_pDead.get();
		break;
	case AIController::AICoreState::PortalGet:
		TargetState = m_pOwner->m_pMove.get();
		break;
	case AIController::AICoreState::PortalDefense:
		TargetState = m_pOwner->m_pMove.get();
		break;
	case AIController::AICoreState::PlayerAttack:
		TargetState = m_pOwner->m_pMove.get();
		break;
	case AIController::AICoreState::Save:
		TargetState = m_pOwner->m_pMove.get();
		break;
	case AIController::AICoreState::none:
		break;
	default:
		break;
	}

	if (TargetState)
	{
		m_pOwner->ChangeState(TargetState);
	}

	m_CurrentCoreState = NextState;
}

bool AIController::PlayerNearby(float Range) const
{
	//Bossの位置を取得.
	D3DXVECTOR3 BossPos = m_pOwner->GetPosition();
	//Playerの位置を取得.
	D3DXVECTOR3 PlayerPos = m_pOwner->GetPlayerPos();
	//方向ベクトルの計算(距離).
	D3DXVECTOR3	Distance = PlayerPos - BossPos;
	
	float DistanceSq = D3DXVec3LengthSq(&Distance);

	return DistanceSq < (Range * Range);
}

bool AIController::LowHealth() const
{
	constexpr float Low_Health_Threshold = 20.0f;
	return m_pOwner->GetEnemyHitPoint() <= Low_Health_Threshold;
}

bool AIController::HasUncapturedPortal() const
{
	return true;
}
