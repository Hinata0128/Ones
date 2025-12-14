#pragma once

//前方宣言.
class Boss;

/**********************************************************************************************
*	ボスのAIの処理を書くクラス
*	優先度を作成するために作成しました.
*	このクラスは継承関係を使用しません.
**/

class AIController final
{
public:
	enum class BossState : byte
	{
		Idol,			//待機.
		Move,			//動き.
		Dead,			//死亡.
		PortalGet,		//ポータルの取得.
		PortalDefense,	//ポータルを守る.
		PlayerAttack,	//プレイヤーに攻撃.
		Save			//退避.
	};
public:
	AIController();
	~AIController();

private:

};