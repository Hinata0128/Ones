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
	//アニメション名のリスト.
	enum class enBossAnimList : byte
	{
		//アニメション名を書く.
		Idol = 0,		//待機.

		RunToIdol,		//走りから待機.
		Run,			//走り中.
		IdolToRun,		//待機から走り.

		Hit,			//被弾.

		ChargeToIdol,	//ため攻撃から待機.
		ChargeAttack,	//ため攻撃中.
		Charge,			//ためている.

		RightMove,		//右に進.
		LeftMove,		//左に進.

		SpecialToIdol,	//特殊攻撃と踏みつけ終了時に待機.
		Special_1,		//特殊攻撃と踏みつけ中.
		Special_0,		//飛ぶ.

		FlinchToIdol,	//怯みから待機.
		Flinch,			//怯み中.
		FlinchParis,	//怯み(パリィ).

		Dead,			//死亡.

		LaserEnd,		//レーザーから待機.
		Laser,			//レーザー中.
		LaserCharge,	//レーザーのため.

		SlashToIdol,	//斬る攻撃から待機.
		Slash,			//斬る攻撃.

		none,			//何もしない.

	};

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

	//死亡しているかどうか.
	bool IsDaed() const { return m_pCurrentState == (BossStateBase*)m_pDead.get(); }
	void Respawn();
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

	D3DXVECTOR3 m_InitialPosition;
};