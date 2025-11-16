#pragma once
#include "..//PlayerState.h"

class Player;
class PlayerContext;

#include "System/00_Manager/07_PlayerAttackManager/PlayerAttackManager.h"
#include "System/02_Singleton/Timer/Timer.h"

/******************************************************************
*	PlayerMoveClass.
* エラーが出たので新しい環境に変更しました
**/

class PlayerMove final
	: public PlayerState
{
public:
	//Playerの右クリックの制御列挙型.
	enum class enStep : byte
	{
		none,
		first,
		run,
		end,
		release_anim,
	};

	//Playerの移動(WASD)の制御列挙型
	enum class enMove : byte
	{
		//待機.
		Idol,		
		//WASDの順番でか書いている.
		ForWard,			//前進(W).
		Left,				//左(A).
		Back,				//後退(S).
		Right,				//右(D).
		//斜め前.
		ForWardAndLeft,		//左斜め前(W+A).
		ForWardAAndRight,	//右斜め前(W+D).
		//斜め後ろ.
		BackAndLeft,		//左斜め後ろ(S+A).
		BackAndRight,		//右斜め後ろ(S+D).
	};
public:
	PlayerMove(Player* pOwner);
	~PlayerMove() override;

	//初期化処理
	void Enter() override;
	//毎フレーム入る処理
	void Execute() override;
	//Stateから出るときに入る処理.
	void Exit() override;

	void Update() override;
	void Draw() override;
	void Init() override;

	//Playerの攻撃関数.
	bool ProcessAttackStep(PlayerContext& ctx);
	//Playerの動作関数.
	void HandleMove(PlayerContext& ctxconst, const D3DXVECTOR3& FB,
		const D3DXVECTOR3& LR);
public:
	//操作の関数,
	enMove GetMoveInput();
private:
	//遠距離攻撃の列挙.
	enStep step;
	//Playerの移動の列挙.
	enMove Move;

	bool AttackType = false;

	//アニメーションの変数.
	bool FastAnim;
};
