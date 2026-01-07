#pragma once
#include "GameObject/StaticMeshObject/StaticMeshObject.h"
#include "GameObject//StaticMeshObject//02_ShotBase//ShotBase.h"

#include "GameObject/SpriteObject/00_Shadow/Shadow.h"

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

public:
	void SetMatrices(const D3DXMATRIX& view, const D3DXMATRIX& proj) {
		if (m_pShadow) {
			// SpriteObject(親)が持っている行列セット関数を呼ぶ
			m_pShadow->SetViewMatrix(view);
			m_pShadow->SetProjMatrix(proj);
		}
	}

private:
	std::unique_ptr<Shadow> m_pShadow;
};