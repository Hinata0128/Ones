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


private:
	//近距離攻撃のクールタイム.
	float m_ShortCoolDown;
	//クールタイム.
	float m_CoolTime;

	//--------------------------------------------------
	// 剣先に当たり判定を実装するためのメンバー変数を作成.
	//--------------------------------------------------
	//剣先のボーン名.
	const char* BoneName = "blade_r_head";
	//剣先の当たり判定の半径
	float Radius = 1.0f;

	//攻撃判定が有効な期間がどうかを確認するフラグ.
	bool m_IsHitActive = false;
	//剣先用の当たり判定オブジェクト.
	BoundingSphere m_SwordHitBox;
};