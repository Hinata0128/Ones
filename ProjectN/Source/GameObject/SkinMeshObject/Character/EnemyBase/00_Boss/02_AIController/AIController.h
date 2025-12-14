#pragma once

//前方宣言.
class Boss;
class BossStateBase;

class BossIdol;
class BossMove;

/**********************************************************************************************
*	ボスのAIの処理を書くクラス
*	優先度を作成するために作成しました.
*	このクラスは継承関係を使用しません.
**/

class AIController final
{
public:
	enum class AICoreState : byte
	{
		Idol,			//待機.
		Move,			//動き.
		Dead,			//死亡.
		PortalGet,		//ポータルの取得.
		PortalDefense,	//ポータルを守る.
		PlayerAttack,	//プレイヤーに攻撃.
		Save,			//退避.

		none			//何もしない.	
	};
public:
	AIController(Boss* owner);
	~AIController();

	//毎フレームの更新.
	void Update();

private:
	//優先順位に基づき。次にとる行動を決定する関数.
	AICoreState DecideNextAction();

	//決定したCoreStateに基づいて、BossのStateを切り替える.
	void ExecuteStateTransition(AICoreState NextState);

	//Playerが指定の範囲にいてるのかの判定.
	bool PlayerNearby(float Range) const;
	//HPが指定の数値を下回ったらどうするか判定する関数.
	bool LowHealth() const;
	//まだ誰もポータルを触っていないかを判定する関数.
	bool HasUncapturedPortal() const;

private:
	Boss* m_pOwner;

	AICoreState m_CurrentCoreState;
};