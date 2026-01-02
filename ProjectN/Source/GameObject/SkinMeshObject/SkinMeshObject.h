#pragma once

#include "GameObject/GameObject.h"
#include "SkinMesh/SkinMesh.h"

/************************************************************
*	スキンメッシュオブジェクトクラス.
**/
class SkinMeshObject
	: public GameObject
{
public:
	SkinMeshObject();
	virtual ~SkinMeshObject() override;

	//CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Update() override;
	virtual void Draw() override;

	//メッシュを接続する.
	void AttachMesh(std::shared_ptr<SkinMesh> pMesh);
	//メッシュを切り離す.
	void DetachMesh();

public:
	//セット・ゲット関数.
	//アニメーションの再生用セット関数.
	void SetIsLoop(const bool& IsLoop);

	//接続メッシュ名を取得.
	const std::weak_ptr<SkinMesh> GetAttachMesh() const
	{
		return m_pMesh;
	}

protected:
	std::shared_ptr<SkinMesh>	m_pMesh;
	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;	//アニメーションコントローラ

	int				m_AnimNo;			//アニメーション番号
	double			m_AnimTime;			//アニメーション経過時間
	double			m_AnimSpeed;		//アニメーション速度
	D3DXVECTOR3		m_BonePos;			//ボーン座標

	bool			m_Loop;	//アニメーションのループ再生.
};
