#pragma once
#include "GameObject//StaticMeshObject//StaticMeshObject.h"	//基底クラス.

class StaticMeshManager;
class ImGuiManager;

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

private:
	//ポータルの範囲に入った時にパーセントを増やす.
	int m_PortalIncrease;

};