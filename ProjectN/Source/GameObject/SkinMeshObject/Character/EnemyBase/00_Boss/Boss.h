#pragma once
#include "GameObject//SkinMeshObject//Character//EnemyBase//EnemyBase.h"	//ベースクラス.
#include "..//..//..//..//..//Collision/BoundingSphere/BoundingSphere.h" // BoundingSphereの定義が必要

#include "..//00_Boss/01_BossStateBase/01_BossMove/BossMove.h" // NomalMove の定義が必要

#include "..//00_Boss/01_BossStateBase/00_BossIdol/BossIdol.h"
#include "..//00_Boss/01_BossStateBase/02_BossDead/BossDead.h"

class BossShotManager;
class Timer;
class BossContext;
class BossStateBase;

class BossAI;

#include "..//..//..//..//StaticMeshObject/01_Portal/Portal.h"



class Boss final
	: public EnemyBase
{
public:
	friend BossContext;
	friend BossAI;
public:
	Boss(std::shared_ptr<Portal> pPortal);
	~Boss() override;

	void Update() override;
	void Draw() override;
	void Init() override;

	void Hit();

public:

	BoundingSphere& GetBoundingSphere() { return m_BSphere; }

	D3DXVECTOR3 GetHitCenter() const;

	void SetEnemyPosition(const D3DXVECTOR3& pos)
	{
		GameObject::SetPosition(pos);
	}

	void SetTargetPos(const D3DXVECTOR3& pos)
	{
		m_pPlayerPos = pos;
	}

	//NomalMoveClassがプレイヤーの位置を取得するためのGet関数.
	const D3DXVECTOR3& GetPlayerPos() const { return m_pPlayerPos; }

	//ポータルの位置を取得する.
	void SetPortalPos(const D3DXVECTOR3& portalpos)
	{
		m_pPortalPos = portalpos;
	}

	//AIの処理で必要になる.
	const D3DXVECTOR3& GetPortalPos() const { return m_pPortalPos; }

	//HPを取得する関数.
	float GetEnemyHitPoint() const { return m_HitPoint; }
public:
	void ChangeState(BossStateBase* state);

	void AutoShot();
	//publicでStateの遷移をしている.
	std::unique_ptr<BossIdol> m_pIdol;
	std::unique_ptr<BossMove> m_pMove;
	std::unique_ptr<BossDead> m_pDead;
private:
	BossShotManager* m_pENShotManager;

	float m_ShotCoolDown;	//弾の発射クールダウンタイマー
	float m_CoolTime;		//弾の発射間隔(1.0f)※コンストラクタで設定している.

	BoundingSphere m_BSphere;//敵用バウンディングスフィア

	D3DXVECTOR3 m_HitCenterOffset;

	//Stateで使用する.
	//時間がなかったのでここで初期化をしている.
	BossStateBase* m_pCurrentState = nullptr;

	std::unique_ptr<BossAI> m_pAI;
};