#pragma once

class Player;

/*************************************************************************
*	攻撃をManagerClassにアタッチするために必要な基底クラス.
**/

class PlayerAttckStateBase
{
public:
	PlayerAttckStateBase();
	virtual ~PlayerAttckStateBase() = default;

	virtual void Enter(Player* player) = 0;
	virtual void ExecuteAttack(Player* player) = 0;
	virtual void Exit(Player* player) = 0;

protected:
};