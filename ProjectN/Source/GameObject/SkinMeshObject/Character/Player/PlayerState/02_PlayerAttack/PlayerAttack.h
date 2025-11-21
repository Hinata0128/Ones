#pragma once
#include "..//PlayerState.h"

class Player;

/************************************************
*	攻撃の基底クラス.
**/

class PlayerAttack
	: public PlayerState
{
public:
	PlayerAttack(Player* pOwner);
	~PlayerAttack() override;

	//初期化処理
	void Enter() override;
	//毎フレーム入る処理
	void Update() override;
	//Stateから出るときに入る処理.
	void Exit() override;

	void Draw() override;
	void Init() override;
protected:

};