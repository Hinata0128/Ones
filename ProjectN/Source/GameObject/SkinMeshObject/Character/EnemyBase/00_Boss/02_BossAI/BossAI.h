#pragma once

/************************************************************************************
*	ボスの行動(AIの制御クラス)
*	優先度を作成してボスの攻撃を実装していきます.
*	優先度順番.
*	1. ボスがポータルの方に移動する.
*	2. ポータルを取得したら防衛状態に入る(防衛の時も攻撃はする).
*	2. ポータルがPlayerに取得されたら攻撃状態に入る(自分が取得していたいから攻撃をする).
*	3. 普通の攻撃.
*	こんな感じの順番で実装していく.
**/

class Portal;
class Boss;

class BossAI
{
public:
	BossAI(
		Boss* pOwner,
		std::shared_ptr<Portal> pPortal);
	~BossAI();

	//ボスのAIを毎フレーム実装.
	void Update();

	//デバッグで表示する.
	void DrawDebugImGui();

private:
	//決定アクション.
	void DecideAction();

	//優先度1: ポータルへの移動関数.
	void MoveToPortl();
	//優先度2: 攻撃(プレイヤーがポータルを取得しているとき).
	void PlayerAttack();
	//優先度2: 防衛(自分がポータルを取得しているとき).
	void Defense();
private:
	std::shared_ptr<Portal> m_pPortal;

	Boss* m_pOwner;
};