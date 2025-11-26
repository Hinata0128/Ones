#include "EnemyNomal.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "System/00_Manager/04_EnemyNomalShotManager/EnemyNomalShotManager.h"
#include "System/02_Singleton/Timer//Timer.h"

#include "NomalContext//NomalContext.h"

#include "GameObject/SkinMeshObject/Character/EnemyBase/EnemyNomal/NomalState/NomalState.h"

constexpr float zero = 0.0f;

EnemyNomal::EnemyNomal()
    : EnemyBase()
    , m_pENShotManager(nullptr)

    , m_pIdol(std::make_unique<NomalIdol>(this))
    , m_pMove(std::make_unique<NomalMove>(this))
{
    SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Enemy);
    auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh, [](SkinMesh*) {});
    AttachMesh(shared_mesh);
    //サイズの変更.
    SetScale(D3DXVECTOR3(0.004f, 0.004f, 0.004f));
    SetPosition(0.0f, 0.0f, 10.0f);

    m_pIdol = std::make_unique<NomalIdol>(this);
    m_pMove = std::make_unique<NomalMove>(this);

    m_pCurrentState = m_pIdol.get();
    m_pCurrentState->Enter();


    m_AnimSpeed = 0.0002f;

    m_pENShotManager = EnemyNomalShotManager::GetInstance();

    m_BSphere.SetRadius(1.0f);

    //当たり判定の位置を変更.
    m_HitCenterOffset = D3DXVECTOR3(zero, 3.0f, zero);

    m_ShotOffset = D3DXVECTOR3(zero, 1.5f, zero);

    // クールタイム初期化
    m_CoolTime = 0.2f; // 1秒ごとに発射
    m_ShotCoolDown = m_CoolTime;
}

EnemyNomal::~EnemyNomal()
{
}

void EnemyNomal::Update()
{
    m_pMesh->SetAnimSpeed(m_AnimSpeed);
    float deltaTime = Timer::GetInstance().DeltaTime();

    m_pENShotManager->Update();


    // クールタイム減算 
    m_ShotCoolDown -= deltaTime;
    if (m_ShotCoolDown < 0.0f) m_ShotCoolDown = 0.0f;

    // ボーン座標取得
    m_pMesh->GetPosFromBone("entotu_Joint", &m_BonePos);

    // アニメーション更新
    m_pAnimCtrl->AdvanceTime(m_AnimSpeed, nullptr);

    if (m_pCurrentState)
    {
        //Idol/Moveを入る.
        m_pCurrentState->Update();
    }
    

    AutoShot();

    EnemyBase::Update();
}

void EnemyNomal::Draw()
{
    m_pMesh->SetAnimSpeed(m_AnimSpeed);
    EnemyBase::Draw();
    m_pENShotManager->Draw();
}

void EnemyNomal::Init()
{
    EnemyBase::Init();
}


void EnemyNomal::ChangeState(NomalState* state)
{
    if (m_pCurrentState == state) return;

    m_pCurrentState->Exit();
    m_pCurrentState = state;
    m_pCurrentState->Enter();
}

void EnemyNomal::AutoShot()
{
    if (m_ShotCoolDown == 0.0f)
    {
        EnemyNomal* pEnemy = this; // 自分自身

        // プレイヤーとの距離計算
        D3DXVECTOR3 toPlayer = pEnemy->GetPlayerPos() - pEnemy->GetPosition();
        float distance = D3DXVec3Length(&toPlayer);

        // 発射距離の設定（例：5.0f以内で発射）
        constexpr float SHOOT_DISTANCE = 20.0f;

        if (distance <= SHOOT_DISTANCE)
        {
            // 変換行列作成
            D3DXMATRIX matS, matR, matT, enemyWorldMatrix;
            D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
            D3DXMatrixRotationYawPitchRoll(&matR, m_Rotation.y, m_Rotation.x, m_Rotation.z);
            D3DXMatrixTranslation(&matT, m_Position.x, m_Position.y, m_Position.z);
            D3DXMatrixMultiply(&enemyWorldMatrix, &matS, &matR);
            D3DXMatrixMultiply(&enemyWorldMatrix, &enemyWorldMatrix, &matT);

            // ボーン座標をワールド座標に変換
            D3DXVECTOR3 worldBonePos;
            D3DXVec3TransformCoord(&worldBonePos, &m_BonePos, &enemyWorldMatrix);

            // 発射位置
            D3DXVECTOR3 shotPos = worldBonePos + m_ShotOffset;

            // プレイヤー方向ベクトル
            D3DXVECTOR3 dir = m_pMove->GetDirectionToPlayer();
            float len = D3DXVec3Length(&dir);
            if (len > 0.001f)
            {
                D3DXVec3Normalize(&dir, &dir);

                // 弾追加
                m_pENShotManager->AddEnemyNomalShot(shotPos, dir);

                // クールタイムリセット
                m_ShotCoolDown = m_CoolTime;
            }
        }
        // else: 距離が遠すぎたら発射しない
    }
}

D3DXVECTOR3 EnemyNomal::GetHitCenter() const
{
    // プレイヤーモデルの位置 (m_Position) にオフセットを加算して返す
    return m_Position + m_HitCenterOffset;
}