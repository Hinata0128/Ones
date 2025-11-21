#pragma once
#include "GameObject//SkinMeshObject//Character//EnemyBase//EnemyBase.h"	//ベースクラス.
#include "..//..//..//..//..//Collision/BoundingSphere/BoundingSphere.h" // BoundingSphereの定義が必要
#include "D3DX9Math.h" // D3DXVECTOR3, D3DXMATRIX などを使用する場合

class EnemyNomalShotManager;
class Timer;

class NomalContext;

//敵クラスに敵のHP現象を作成.
//将来的には、jsonファイルでHP関係を管理する.
//このクラスで敵の攻撃範囲を作成して攻撃を実装する.

class EnemyNomal final
	: public EnemyBase // public継承を推奨 (private継承の場合は、外部からのアクセスに注意)
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

	void SetEnemyPosition(const D3DXVECTOR3& pos) {
		GameObject::SetPosition(pos);
	}

	void SetTargetPos(const D3DXVECTOR3& pos)
	{
		// プレイヤー位置をメンバ変数 m_pPlayerPos に保存する
		m_pPlayerPos = pos;
	}

private:
	void AutoShot();

private:
	EnemyNomalShotManager* m_pENShotManager;

	D3DXVECTOR3 m_ShotOffset;       // 弾の発射位置オフセット

	float m_ShotCoolDown;           // 弾の発射クールダウンタイマー
	float m_CoolTime;               // 弾の発射間隔（1.0fなど）

	// *****************************

	BoundingSphere m_BSphere;       // 敵用バウンディングスフィア

	// m_HitCenterOffset は GetHitCenter() で利用されているため追加
	D3DXVECTOR3 m_HitCenterOffset;

	float m_RotationSpeed;          // 円軌道に使用
	float m_RotationDirection;      // 円軌道に使用
	float m_RotationAngle;          // 円軌道に使用

	float m_ApproachSpeed;			//追尾の速度調整用.

	// プレイヤーへ向かう方向ベクトルを保持するメンバ変数を追加
	D3DXVECTOR3 m_DirectionToPlayer;
};