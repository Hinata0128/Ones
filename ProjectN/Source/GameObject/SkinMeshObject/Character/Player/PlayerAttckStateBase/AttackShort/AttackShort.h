#pragma once
#include "..//PlayerAttckStateBase.h"	//基底クラス.

#include "..//..//..//..//..//../Collision/BoundingSphere/BoundingSphere.h"
//------前方宣言------
class Player;
class PlayerContext;
class Timer;

/**********************************************************
*	PlayerShortClass(近距離攻撃用のクラス).
*	PlayerAttackStateBaseを継承させる
*	PlayerAttackManagerでポリモーフィズムを使用してPlayerClassへ操作をまかしている.
**/

class AttackShort
	: public PlayerAttckStateBase
{
public:
	AttackShort();
	~AttackShort() override;

	void Enter(Player* player) override;
	void ExecuteAttack(Player* player) override;
	void Exit(Player* player) override;

	void Draw(Player* player);

	// 攻撃判定の状態を取得する関数
	bool IsHitActive() const { return m_IsHitActive; }
	const BoundingSphere& GetHitBox() const { return m_SwordHitBox; }

private:
	//近距離攻撃のクールタイム.
	float m_ShortCoolDown;
	//クールタイム.
	float m_CoolTime;

	//剣先の当たり判定の半径
	float Radius = 1.5f;

	//攻撃判定が有効な期間がどうかを確認するフラグ.
	bool m_IsHitActive = false;
	//剣先用の当たり判定オブジェクト.
	BoundingSphere m_SwordHitBox;
};