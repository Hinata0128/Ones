#include "BossAI.h"

#include "GameObject//StaticMeshObject//01_Portal//Portal.h"

#include "GameObject/SkinMeshObject/Character/EnemyBase/00_Boss/Boss.h"

BossAI::BossAI(Boss* pOwner)
	: m_pPortal	(std::make_shared<Portal>())
	, m_pOwner(pOwner)
{
}

BossAI::~BossAI()
{
}

void BossAI::Update()
{
	PortalToBoss();
}

void BossAI::AIUpdate()
{
}

//1. 優先度1の処理を書く関数.
void BossAI::PortalToBoss()
{
	//敵の位置を取得する.
	D3DXVECTOR3 BossPos = m_pOwner->GetPosition();
	//ポータルの位置を取得する.
	D3DXVECTOR3 PortalPos = m_pPortal->GetPosition();

	//ポータルの位置 - 敵の位置の取得.
	D3DXVECTOR3 Length = PortalPos - BossPos;

	//ポータルから敵への長さ(距離を入手する).
	float Pos_Lenght = D3DXVec3Length(&Length);

	//ローカル変数(ポータルを触る距離).
	float PortalGet = 5.0f;
}
