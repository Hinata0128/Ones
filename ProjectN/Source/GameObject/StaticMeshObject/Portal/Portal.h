#pragma once
#include "GameObject//StaticMeshObject//StaticMeshObject.h"	//基底クラス.

class StaticMeshManager;
class ImGuiManager;
class Player;

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
	void SetPlayer(std::shared_ptr<Player> player) { m_pPlayer = player; }
private:
	//ポータルの範囲に入った時にパーセントを増やす.
	float m_PortalIncreaseF;
	int m_PortalIncrease;

	//Playerの位置の取得のためにメンバ変数としてしようする.
	std::weak_ptr<Player> m_pPlayer;
};