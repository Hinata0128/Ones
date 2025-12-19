#include "BossShot.h"
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"


BossShot::BossShot()
{
    AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::EnemyBullet));

    Init();
}

BossShot::~BossShot() {}

void BossShot::Update()
{
    constexpr float MaxRange = 9999.0f;
    if (!m_IsActive) return;

    if (m_Disp)
    {
        m_Position += m_Direction * m_Speed;
        if (D3DXVec3Length(&m_Position) >= MaxRange) Init();
        UpdateBPosition();
    }
}

void BossShot::Draw()
{
    ShotBase::Draw(); 
}

void BossShot::Init()
{
    ShotBase::Init();

    m_Scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
    if (m_BSphere) m_BSphere->SetRadius(0.3f); 
}

void BossShot::Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed)
{
    m_Position = pos;
    m_Direction = direction;
    m_Speed = speed;
    m_Disp = true;
    m_IsActive = true;

    if (m_BSphere) m_BSphere->SetPosition(m_Position);
}
