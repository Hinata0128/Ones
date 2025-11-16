#pragma once
#include "..//PlayerAttckStateBase.h"	//基底クラス.

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

private:
	//近距離攻撃のクールタイム.
	float m_ShortCoolDown;
	//クールタイム.
	float m_CoolTime;
};