#include "Boss.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "System/00_Manager/04_BossShotManager/BossShotManager.h"
#include "System/02_Singleton/Timer/Timer.h"
#include "..//00_Boss/00_BossContext/BossContext.h"
#include "..//00_Boss/01_BossStateBase/BossStateBase.h"

#include "02_BossAI/BossAI.h"

constexpr float zero = 0.0f;

Boss::Boss(std::shared_ptr<Portal> pPortal)
    : EnemyBase()
    , m_pENShotManager(nullptr)
    , m_pIdol(std::make_unique<BossIdol>(this))
    , m_pMove(std::make_unique<BossMove>(this))
    , m_pDead(std::make_unique<BossDead>(this))
    , m_pAI(std::make_unique<BossAI>(this, pPortal))
{
    SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Enemy);
    auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh, [](SkinMesh*) {});
    AttachMesh(shared_mesh);

    // サイズと位置
    SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
    SetPosition(20.0f, 0.f, 15.f);

    // ステートセット
    m_pCurrentState = m_pIdol.get();
    if (m_pCurrentState)
    {
        m_pCurrentState->Enter();
    }

    //アニメーションの再生速度.
    m_AnimSpeed = 1.0f / 60.0f;

    m_pENShotManager = BossShotManager::GetInstance();

    m_BSphere.SetRadius(1.0f);
    m_HitCenterOffset = D3DXVECTOR3(0.0f, 3.0f, 0.0f);

    m_CoolTime = 0.8f;
    m_ShotCoolDown = m_CoolTime;

    Init();
}

Boss::~Boss()
{
}

void Boss::Init()
{
    m_HitPoint = 100.0f;

    //待機アニメーションを再生.
    const int IDLE_ANIM = 0;
    if (m_pMesh && m_pAnimCtrl)
    {
        m_pMesh->ChangeAnimSet(IDLE_ANIM, m_pAnimCtrl);
        m_pMesh->SetAnimSpeed(m_AnimSpeed);
    }


    EnemyBase::Init();
}

void Boss::Update()
{
    float deltaTime = Timer::GetInstance().DeltaTime();

    m_pENShotManager->Update();

    // クールタイム
    m_ShotCoolDown -= deltaTime;

    if (m_ShotCoolDown < 0.0f)
    {
        m_ShotCoolDown = 0.0f;
    }

    m_pAnimCtrl->AdvanceTime(m_AnimSpeed, nullptr);

    if (m_pCurrentState)
    {
        m_pCurrentState->Update();
    }


    //今敵の攻撃が邪魔だからコメントで消しておく.
    //AutoShot();

    m_pAI->Update();



    // ボーン座標取得
    m_pMesh->GetPosFromBone("boss_head", &m_BonePos);

    EnemyBase::Update();
}

void Boss::Draw()
{
    m_pMesh->SetAnimSpeed(m_AnimSpeed);

    EnemyBase::Draw();
    m_pENShotManager->Draw();
    m_pAI->DrawDebugImGui();
}

void Boss::Hit()
{
    m_HitPoint -= 10.0f;

    if (m_HitPoint <= 0.0f)
    {
        m_HitPoint = 0.0f;

        BossStateBase* nextState = m_pDead.get();

        if (m_pCurrentState != nextState)
        {
            if (m_pCurrentState) m_pCurrentState->Exit();
            m_pCurrentState = nextState;
            if (m_pCurrentState) m_pCurrentState->Enter();
        }
    }
}

void Boss::ChangeState(BossStateBase* state)
{
    if (m_pCurrentState == state) return;

    if (m_pCurrentState) m_pCurrentState->Exit();

    m_pCurrentState = state;

    if (m_pCurrentState) m_pCurrentState->Enter();
}

void Boss::AutoShot()
{
    if (m_ShotCoolDown == 0.0f)
    {
        //プレイヤーとの距離計算
        D3DXVECTOR3 toPlayer = GetPlayerPos() - GetPosition();
        float distance = D3DXVec3Length(&toPlayer);

        //発射距離の設定
        //個々の数値を変更すると発射までの距離が変更できる.
        constexpr float SHOOT_DISTANCE = 20.0f;

        if (distance <= SHOOT_DISTANCE)
        {
            //変換行列作成
            D3DXMATRIX matS, matR, matT, enemyWorldMatrix;
            D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
            D3DXMatrixRotationYawPitchRoll(&matR, m_Rotation.y, m_Rotation.x, m_Rotation.z);
            D3DXMatrixTranslation(&matT, m_Position.x, m_Position.y, m_Position.z);
            D3DXMatrixMultiply(&enemyWorldMatrix, &matS, &matR);
            D3DXMatrixMultiply(&enemyWorldMatrix, &enemyWorldMatrix, &matT);

            //ボーン座標をワールド座標に変換
            D3DXVECTOR3 worldBonePos;
            D3DXVec3TransformCoord(&worldBonePos, &m_BonePos, &enemyWorldMatrix);

            //発射位置
            D3DXVECTOR3 shotPos = GetPosition() + D3DXVECTOR3(0.0f, 3.0f, 0.0f);

            //プレイヤー方向ベクトル
            D3DXVECTOR3 dir = m_pMove->GetDirectionToPlayer();
            float len = D3DXVec3Length(&dir);
            if (len > 0.001f)
            {
                D3DXVec3Normalize(&dir, &dir);

                //弾追加
                m_pENShotManager->AddEnemyNomalShot(shotPos, dir);

                //クールタイムリセット
                m_ShotCoolDown = m_CoolTime;
            }
        }
    }
}

D3DXVECTOR3 Boss::GetHitCenter() const
{
    return m_Position + m_HitCenterOffset;
}
