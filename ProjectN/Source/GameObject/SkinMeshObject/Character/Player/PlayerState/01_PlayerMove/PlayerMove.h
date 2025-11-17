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
	//Playerの左クリックの制御列挙型.
	enum class enLeftStep : byte
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

	//Playerの右クリックの攻撃関数.
	bool RbuttonAttackStep(PlayerContext& ctx);
	//Playerの左クリックの攻撃関数.
	//引数を書かないといけないけれども一旦関数を作成するために書いていない.
	bool LButtonAttackStep(PlayerContext& ctx);

	//Playerの動作[GetAsyncKeyState()]を書く関数.
	void HandleMove(
		PlayerContext& ctxconst,
		const D3DXVECTOR3& WS,
		const D3DXVECTOR3& AD);
public:
	//操作の関数,
	enMove GetMoveInput();
private:
	//遠距離攻撃の列挙.
	enStep step;
	//近距離攻撃の列挙.
	enLeftStep LStep;
	//Playerの移動の列挙.
	enMove Move;

	bool AttackType = false;

	//アニメーションの変数.
	bool FastAnim;
};
