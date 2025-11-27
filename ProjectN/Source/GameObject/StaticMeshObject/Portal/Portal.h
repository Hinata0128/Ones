#pragma once
#include "GameObject//StaticMeshObject//StaticMeshObject.h"	//基底クラス.

class StaticMeshManager;

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

private:

};