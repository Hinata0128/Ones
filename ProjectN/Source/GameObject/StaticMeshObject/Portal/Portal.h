#pragma once
#include "GameObject//StaticMeshObject//StaticMeshObject.h"	//基底クラス.

class StaticMeshManager;
class ImGuiManager;
class Player;
class EnemyNomal;

/************************************************************
*	陣地を守用のポータル
*	取り合いのゲームだから1個で作成していく.
*	表示をするのが目的なのでUpdate()を回さない.
**/

class Portal final
	: public StaticMeshObject
{
public:
	Portal();
	~Portal() override;

	void Update() override;
	void Draw() override;
	//初期化関数.
	void Init();
public:
	//===============================================================================
	// ポータルを触るときにPlayerとEnemyNomalの位置が必要なので
	// GameMainClassにあるm_pPlayerとm_pEnemyNomalの座標を取得するように
	// PortalClassにSet関数を作成しました.
	//===============================================================================
	//Playerの位置を設定するSet関数.
	void SetPlayer(std::shared_ptr<Player> player) { m_pPlayer = player; }
	//敵の位置を設定するSet関数.
	void SetEnemyNomal(std::shared_ptr<EnemyNomal> nomal) { m_pEnemy = nomal; }
private:
	//ポータルの範囲に入った時にパーセントを増やす.
	float m_PortalIncreaseF;
	int m_PortalIncrease;

	//Playerの位置の取得のためにメンバ変数としてしようする.
	std::weak_ptr<Player> m_pPlayer;
	//敵の位置の取得のためにメンバ変数として使用します
	std::weak_ptr<EnemyNomal> m_pEnemy;
};