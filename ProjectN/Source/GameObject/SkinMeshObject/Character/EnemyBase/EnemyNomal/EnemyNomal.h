#pragma once
#include "GameObject//SkinMeshObject//Character//EnemyBase//EnemyBase.h"	//ベースクラス.
#include "..//..//..//..//..//Collision/BoundingSphere/BoundingSphere.h" // BoundingSphereの定義が必要

#include "..//EnemyNomal/NomalState/01_NomalMove/NomalMove.h" // NomalMove の定義が必要

class EnemyNomalShotManager;
class Timer;
class NomalContext;

class EnemyNomal final
	: public EnemyBase
{
public:
	friend NomalContext;

public:
	EnemyNomal();
	~EnemyNomal() override;

	void Update() override;
	void Draw() override;
	void Init() override;

public:
	//W・Sの前進後退用関数.
	D3DXVECTOR3 Enemy_WS() const;

	BoundingSphere& GetBoundingSphere() { return m_BSphere; }

	D3DXVECTOR3 GetHitCenter() const;

	// SetEnemyPosition は外部での使用のため残す
	void SetEnemyPosition(const D3DXVECTOR3& pos) {
		GameObject::SetPosition(pos);
	}

	void SetTargetPos(const D3DXVECTOR3& pos)
	{
		m_pPlayerPos = pos;
	}

	// NomalMove クラスがプレイヤーの位置を取得するためのアクセサ
	const D3DXVECTOR3& GetPlayerPos() const { return m_pPlayerPos; }

private:
	void AutoShot();

private:
	EnemyNomalShotManager* m_pENShotManager;

	D3DXVECTOR3 m_ShotOffset;// 弾の発射位置オフセット

	float m_ShotCoolDown;// 弾の発射クールダウンタイマー
	float m_CoolTime;// 弾の発射間隔（1.0fなど）

	BoundingSphere m_BSphere;// 敵用バウンディングスフィア

	D3DXVECTOR3 m_HitCenterOffset;

	// ★移動ロジック用のメンバ変数は NomalMove に移動したため削除★

	std::shared_ptr<NomalMove> m_pMove;
};