#pragma once
#include "..//NomalState.h"

/******************************************************************
*	敵の死亡クラス.
**/

class EnemyNomal;
class EnemyContext;

class NomalDead final
	: public NomalState
{
public:
	NomalDead(EnemyNomal* pOwner);
	~NomalDead();

	//初期化処理
	void Enter() override;
	//毎フレーム入る処理
	void Update() override;
	//Stateから出るときに入る処理.
	void Exit() override;

	void Draw() override;
	void Init() override;


private:

};