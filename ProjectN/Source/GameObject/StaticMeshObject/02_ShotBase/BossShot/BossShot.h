#pragma once
#include "GameObject//StaticMeshObject//02_ShotBase//ShotBase.h"

class StaticMeshManager;

/**********************************************************************
*	“G’Êí’eƒNƒ‰ƒX.
**/

class BossShot
	: public ShotBase
{
public:
	BossShot();
	~BossShot() override;

	void Update() override;
	void Draw() override;

	//’e‚Ì‰Šú‰».
	void Init() override;

	void Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed) override;

private:

};