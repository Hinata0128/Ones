#pragma once
#include "GameObject/StaticMeshObject/StaticMeshObject.h"
#include "GameObject//StaticMeshObject//ShotBase//ShotBase.h"

class StaticMeshManager;

/******************************************************************
*	ショットクラス.
**/

class PShot
	: public ShotBase
{
public:
	PShot();
	~PShot() override;

	void Update() override;
	void Draw() override;

	//弾の初期化.
	void Init();

	// ShotBase で純粋仮想にした Reload を実装
	void Reload(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& direction,
		float speed) override;

private:
};