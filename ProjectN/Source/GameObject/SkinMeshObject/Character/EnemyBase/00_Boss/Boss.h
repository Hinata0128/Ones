#pragma once
#include "GameObject//SkinMeshObject//Character//EnemyBase//EnemyBase.h"	//ベースクラス.
#include "..//..//..//..//..//Collision/BoundingSphere/BoundingSphere.h" // BoundingSphereの定義が必要

#include "..//00_Boss/NomalState/01_NomalMove/NomalMove.h" // NomalMove の定義が必要

#include "..//00_Boss/NomalState/00_NomalIdol/NomalIdol.h"
#include "..//00_Boss/NomalState/02_NomalDead/NomalDead.h"

class BossShotManager;
class Timer;
class NomalContext;
class NomalState;

class Boss final
	: public EnemyBase
{
public:
	friend NomalContext;
public:
	Boss();
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

	//HPを取得する関数.
	float GetEnemyHitPoint() const { return m_HitPoint; }
public:
	void ChangeState(NomalState* state);

	void AutoShot();
	//publicでStateの遷移をしている.
	std::unique_ptr<NomalIdol> m_pIdol;
	std::unique_ptr<NomalMove> m_pMove;
	std::unique_ptr<NomalDead> m_pDead;
private:
	BossShotManager* m_pENShotManager;

	float m_ShotCoolDown;	//弾の発射クールダウンタイマー
	float m_CoolTime;		//弾の発射間隔(1.0f)※コンストラクタで設定している.

	BoundingSphere m_BSphere;//敵用バウンディングスフィア

	D3DXVECTOR3 m_HitCenterOffset;

	//Stateで使用する.
	//時間がなかったのでここで初期化をしている.
	NomalState* m_pCurrentState = nullptr;
};