#pragma once
#include "GameObject//SkinMeshObject//Character//Character.h"


/************************************************************************
*	敵ベースクラス.
*	これを基底クラスにして作成していきます.
**/

class EnemyBase
	: public Character
{
public:
	EnemyBase();
	virtual ~EnemyBase() override;

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Init() override;

public:
	//敵がPlayerの位置を取得するために必要なSet関数を作成.
	void SetTargetPos(const D3DXVECTOR3 Player_Pos);
	//Playerの位置を取得するGet関数
	D3DXVECTOR3 GetTargetPos();
protected:
	D3DXVECTOR3 m_pPlayerPos;

	//ポータルの位置の取得.
	D3DXVECTOR3 m_pPortalPos;
};