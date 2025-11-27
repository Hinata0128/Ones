#include "Portal.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

Portal::Portal()
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Portal));

	//ポータルの位置を変更.
	D3DXVECTOR3 Pos		= { 0.0f, 0.0f, 5.0f };
	//ポータルのサイズ変更.
	D3DXVECTOR3 Scale	= { 1.5f, 1.5f, 1.5f };

	SetPosition(Pos);
	SetScale(D3DXVECTOR3(Scale));
}

Portal::~Portal()
{
}

void Portal::Update()
{
	StaticMeshObject::Update();
}

void Portal::Draw()
{
	StaticMeshObject::Draw();
}
