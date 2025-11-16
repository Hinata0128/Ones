#include "EnemyNomalShot.h"
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"


EnemyNomalShot::EnemyNomalShot()
{
    AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::EnemyBullet));

    //


    Init();
}

EnemyNomalShot::~EnemyNomalShot() {}

void EnemyNomalShot::Update()
{
    constexpr float MaxRange = 30.0f;
    if (!m_IsActive) return;

    if (m_Disp)
    {
        m_vPosition += m_Direction * m_Speed;
        if (D3DXVec3Length(&m_vPosition) >= MaxRange) Init();
        UpdateBPosition();
    }
}

void EnemyNomalShot::Draw() { ShotBase::Draw(); }

void EnemyNomalShot::Init()
{
    ShotBase::Init();

    m_vScale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
    if (m_BSphere) m_BSphere->SetRadius(0.3f); 
}

void EnemyNomalShot::Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed)
{
    m_vPosition = pos;
    m_Direction = direction;
    m_Speed = speed;
    m_Disp = true;
    m_IsActive = true;

    if (m_BSphere) m_BSphere->SetPosition(m_vPosition);
}
