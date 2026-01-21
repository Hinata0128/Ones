#include "Boss.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "System/00_Manager/04_BossShotManager/BossShotManager.h"
#include "System/02_Singleton/00_Timer/Timer.h"
#include "..//00_Boss/00_BossContext/BossContext.h"
#include "..//00_Boss/01_BossStateBase/BossStateBase.h"

#include "02_Com/Com.h"

#include "GameObject/01_SpriteObject/00_Shadow/Shadow.h"

#include "SceneManager/SceneManager.h"

constexpr float zero = 0.0f;

Boss::Boss(std::shared_ptr<Portal> pPortal)
    : Character()
    , m_pENShotManager(nullptr)
    , m_pIdol(std::make_unique<BossIdol>(this))
    , m_pMove(std::make_unique<BossMove>(this))
    , m_pDead(std::make_unique<BossDead>(this))
    , m_pCom(std::make_unique<Com>(this, pPortal))
    
    , m_InitialPosition {}

    , m_CurrentRound    (1) //開始はラウンド1からのため.

    , m_IsVisible   (true)

    , m_IsFrozen    (false)

    , m_pShadow     ( std::make_shared<Shadow>() )
    , m_pPlayer     ( std::make_unique<Player>() )
{
    SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Enemy);
    auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh, [](SkinMesh*) {});
    AttachMesh(shared_mesh);

    // サイズと位置
    SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));

    m_InitialPosition = D3DXVECTOR3(20.0f, 0.f, 15.f);
    //m_InitialPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    SetPosition(m_InitialPosition);
    //SetPosition(0.0f, 0.0f, 0.0f);

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
    m_HitCenterOffset = D3DXVECTOR3(0.0f, 3.5f, 0.0f);

    m_pCom->DecideDifficultyByRound(m_CurrentRound);

    Init();
}

Boss::~Boss()
{
}

void Boss::Init()
{
    m_HitPoint = 100.0f;

    int score = SceneManager::GetInstance()->GetPlayerScore() + SceneManager::GetInstance()->GetEnemyScore();
    m_CurrentRound = score + 1;

    if (m_pCom)
    {
        m_pCom->DecideDifficultyByRound(m_CurrentRound);
    }

    //待機アニメーションを再生.
    const int IDLE_ANIM = 0;
    if (m_pMesh && m_pAnimCtrl)
    {
        m_pMesh->ChangeAnimSet(IDLE_ANIM, m_pAnimCtrl);
        m_AnimNo = IDLE_ANIM;
        m_pMesh->SetAnimSpeed(m_AnimSpeed);
    }

    m_pShadow->Create();
    m_pShadow->SetTargetShadowPos(this);

    Character::Init();
}

void Boss::Update()
{
    float deltaTime = Timer::GetInstance().DeltaTime();

    if (m_IsFrozen)
    {
        Character::Update();
        return;
    }

    m_pENShotManager->Update();

    // クールタイム
    m_ShotCoolDown -= deltaTime;

    if (m_ShotCoolDown < 0.0f)
    {
        m_ShotCoolDown = 0.0f;
    }

    //m_pAnimCtrl->AdvanceTime(m_AnimSpeed, nullptr);
    
    if (!IsDaed())
    {
        m_pCom->Update();
    }

    // ボーン座標取得
    m_pMesh->GetPosFromBone("boss_head", &m_BonePos);

    m_pShadow->Update();

    Character::Update();
}

void Boss::Draw()
{
    m_pMesh->SetAnimSpeed(m_AnimSpeed);
    m_pShadow->Draw();
    Character::Draw();
    m_pCom->DrawDebugImGui();
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

            SetVisible(false);
        }
    }
}

bool Boss::CanShot() const
{
    return m_ShotCoolDown <= 0.0f;
}

void Boss::RequestShot()
{
    if (!CanShot())
    {
        return;
    }

    AutoShot();
    //クールタイムのリセット.
    //ToDo : 攻撃できるように.
    m_ShotCoolDown = m_CoolTime;
}

void Boss::SetShotInterval(float time)
{
    m_CoolTime = time;
    //m_ShotCoolDown = time;
}

void Boss::StartNextRound(int nextround)
{
    m_CurrentRound = nextround;

    //Comクラスに次のラウンドを知らせる.
    if (m_pCom)
    {
        m_pCom->DecideDifficultyByRound(m_CurrentRound);
    }
    Respawn();
}

void Boss::Respawn()
{
    // 1. 位置と体力の復帰
    SetPosition(m_InitialPosition);
    m_HitPoint = 100.0f;

    // 2. ステートを強制的に待機(Idol)に戻す
    // ※これをしないと Update で死亡状態のまま処理されてしまいます
    m_pCurrentState = m_pIdol.get();
    if (m_pCurrentState)
    {
        m_pCurrentState->Enter();
    }

    // 3. アニメーションを強制的に初期化
    const int IDLE_ANIM = 0;
    if (m_pMesh && m_pAnimCtrl)
    {
        // 条件(m_AnimNo == -1)を削除し、強制的に待機アニメーションをセット
        m_pMesh->ChangeAnimSet(IDLE_ANIM, m_pAnimCtrl);
        m_AnimNo = IDLE_ANIM;
        m_pMesh->SetAnimSpeed(m_AnimSpeed);
    }

    SetVisible(true);

    // 4. その他の初期化（Init内で二重にSetPositionなどしないよう注意）
    // Init() の中身が重複している場合は整理が必要ですが、現状のコードなら維持でOKです
    Init();
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
    if (!m_pPlayer) return;

    // 1. 距離チェック（足元基準でOK）
    D3DXVECTOR3 playerPos = GetPlayerPos();
    D3DXVECTOR3 toPlayer = playerPos - GetPosition();
    float distance = D3DXVec3Length(&toPlayer);

    constexpr float SHOOT_DISTANCE = 20.0f;

    if (distance <= SHOOT_DISTANCE)
    {
        // 2. 発射位置（ユーザー様のご指定通り固定）
        D3DXVECTOR3 shotPos = GetPosition() + D3DXVECTOR3(0.0f, 3.0f, 0.0f);

        // 3. 狙うターゲットの位置（プレイヤーのボーン）
        D3DXVECTOR3 targetPos;
        if (!m_pPlayer->GetBonePosition("Bone002", &targetPos))
        {
            // ボーンが取得できなかった時の保険（少し高い位置を狙う）
            targetPos = playerPos + D3DXVECTOR3(0.0f, 2.0f, 0.0f);
        }

        // 4. 方向ベクトルの計算を修正
        // 「固定の発射位置」から「動いているボーン」へ向かうベクトルを作成
        D3DXVECTOR3 finalDir = targetPos - shotPos;
        float len = D3DXVec3Length(&finalDir);

        if (len > 0.001f)
        {
            // 正規化して長さを1にする
            D3DXVec3Normalize(&finalDir, &finalDir);

            // 弾を追加（計算した finalDir を使用）
            m_pENShotManager->AddEnemyNomalShot(shotPos, finalDir);
        }
    }
}

D3DXVECTOR3 Boss::GetHitCenter() const
{
    return m_Position + m_HitCenterOffset;
}
