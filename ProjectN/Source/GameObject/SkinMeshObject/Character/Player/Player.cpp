#include "Player.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"

#include "System/00_Manager/02_PShotManager/PShotManager.h"

#include "System/02_Singleton/Timer//Timer.h"

#include "System/00_Manager/03_ImGuiManager/ImGuiManager.h"

//---------------------------------------------
// PlayerStateのインクルード.
//---------------------------------------------
#include "..//Player/PlayerState/PlayerState.h"
#include "GameObject//SkinMeshObject//Character//Player//PlayerState//00_PlayerIdol//PlayerIdol.h"
#include "GameObject//SkinMeshObject//Character//Player//PlayerContext//PlayerContext.h"
#include "..//Player/PlayerState/03_PlayerDead/PlayerDead.h"

#include "..//Player/PlayerState/02_PlayerPortalAnim/PlayerPortalAnim.h"


Player::Player()
    :Character()
    , m_pShotManager    (PShotManager::GetInstance())
    , m_pAttackManager(std::make_unique<PlayerAttackManager>(this))
    , m_pPlayerIdol     (std::make_shared<PlayerIdol>(this))
    , m_pPlayerDead     (std::make_shared<PlayerDead>(this))
    , m_pPlayerAnim     (std::make_shared<PlayerPortalAnim>(this))

    , m_pCurrentState   (nullptr)

{
	SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Player);
	auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh,[](SkinMesh*){});
	//AttachMesh に shared_ptr を渡す
	AttachMesh(shared_mesh);
    
    //Game開始時にIdolclassに入る.
    //Idolclassの中でMoveとAttackを呼んでいる.
    m_pCurrentState = m_pPlayerIdol.get();

    if (m_pCurrentState) 
    {
        m_pCurrentState->Enter();
    }
    Init();
}

Player::~Player()
{
}

void Player::Update()
{
    //最初に書いておかないと、ポータルを触った時に弾が進まない.
    if (m_pShotManager)
    {
        m_pShotManager->Update(); // 弾の移動処理を実行
    }


    if (IsCapturingState())
    {
        //タイマーの更新.
        m_CaptureTimer -= Timer::GetInstance().DeltaTime();

        if (m_CaptureTimer <= 0.0f)
        {
            //タイマー終了: StateをIdolに戻す.
            PlayerState* nextState = m_pPlayerIdol.get();
            
            if (m_pCurrentState) {
                m_pCurrentState->Exit(); // 古いステート(PortalAnim)の終了処理
            }
            m_pCurrentState = nextState; // ステートを切り替え
            if (m_pCurrentState) {
                m_pCurrentState->Enter(); // 新しいステート(Idol)の開始処理
            }

            m_CaptureTimer = 0.0f;
        }
        return;
    }



    //PlayerAttackManager の更新
    if (m_pAttackManager)
    {
        m_pAttackManager->Update();
    }


    //これでその時のStateに入る.
    if (m_pCurrentState)
    {
        m_pCurrentState->Update();
    }

    //アニメーション更新
    m_pAnimCtrl->AdvanceTime(m_AnimSpeed, nullptr);

#ifdef _DEBUG

    ImGui::Begin(JAPANESE("playerのポジションを変更"));
    ImGui::InputFloat3("pos", m_Position);
    ImGui::End();
#endif

    //ボーン座標の取得.
    //弾の発射位置を計算するために使用.
    m_pMesh->GetPosFromBone("blade_l_head", &m_BonePos);
    //m_pMesh->GetPosFromBone("boss_head", &m_BonePos);


    //基底クラスの更新処理.
    Character::Update();
}

void Player::Draw()
{
	//m_pMesh->SetAnimSpeed(m_AnimSpeed);
    m_pAttackManager->Draw();
	Character::Draw();
}

void Player::Init()
{
    //Playerのサイズの変更.
    SetScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
    //SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
    //Playerの初期位置の変更.
    SetPosition(-20.f, 0.f, -15.f);
    

    //Playerの体力.
    m_HitPoint = 100.0f;

    //アニメーションの速度.
    m_AnimSpeed = 1.0f / 60.0f;
    //当たり判定のサイズ
    m_BSphere.SetRadius(0.7f);

    //当たり判定の位置を変更.
    m_HitCenterOffset = D3DXVECTOR3(0.0f, 2.5f, 0.0f);

    Character::Init();
}

void Player::Hit()
{
    constexpr float zero = 0.0f;
    constexpr float ten = 10.0f;
    //Playerのダメージ実装用のローカル変数.
    constexpr float Dead_HP = zero;
    constexpr float Damege_Hit = ten;

    //くらった時のHPの減少.
    m_HitPoint -= Damege_Hit;
    if (m_HitPoint <= Dead_HP)
    {
        //ここでPlayerのHPが0になったらPlayerDeadClassを呼ぶ.
        m_HitPoint = Dead_HP;

        // nextState という名前に修正し、ステート遷移ロジックを実装
        PlayerState* nextState = m_pPlayerDead.get();

        if (m_pCurrentState != nextState)
        {
            if (m_pCurrentState) {
                m_pCurrentState->Exit();    //古いステートの終了処理
            }
            m_pCurrentState = nextState;    //ステートを切り替え
            if (m_pCurrentState) {
                m_pCurrentState->Enter();   //新しいステートの開始処理
            }
        }
    }
}

D3DXVECTOR3 Player::GetHitCenter() const
{
    //プレイヤーモデルの位置 (m_Position) にオフセットを加算して返す
    return m_Position + m_HitCenterOffset;
}

void Player::InitializePlayerMove()
{
    m_pAttackManager->CleanUpState(PlayerAttackManager::enAttack::Long);
    m_pPlayerIdol->Init();
}

D3DXVECTOR3 Player::Player_WS(float RotationY) const
{
    D3DXVECTOR3 dir;
    dir.x = sinf(RotationY);
    dir.y = 0.0f;
    dir.z = cosf(RotationY);
    return dir;
}

D3DXVECTOR3 Player::Player_AD(float RotationY) const
{
    D3DXVECTOR3 dir;
    dir.x = cosf(RotationY);
    dir.y = 0.0f;
    dir.z = -sinf(RotationY);
    return dir;
}

void Player::ChangeAttackType(PlayerAttackManager::enAttack type)
{
    if (m_pAttackManager)
    {
        // 実際のステート切り替え処理はAttackManagerに任せる
        m_pAttackManager->ChangeAttackState(type);
    }
}

void Player::CleanUpAttackState(PlayerAttackManager::enAttack type)
{
    if (m_pAttackManager)
    {
        // 実際のステート切り替え処理はAttackManagerに任せる
        m_pAttackManager->CleanUpState(type);
    }

}

bool Player::GetBonePosition(const char* boneName, D3DXVECTOR3* outPos) const
{
    if (m_pMesh)
    {
        return m_pMesh->GetPosFromBone(boneName, outPos);
    }

    return false;
}

D3DXVECTOR3 Player::GetShortAttackCenter() const
{
    D3DXVECTOR3 center = m_BonePos;

    // 剣の先端方向へ 0.3 くらい前に出す（Z 方向はモデルの向きで変化）
    D3DXVECTOR3 offset(0.f, 0.f, 0.3f);

    // プレイヤーの回転に合わせて offset を回転
    D3DXMATRIX rot;
    D3DXMatrixRotationY(&rot, m_Rotation.y);
    D3DXVec3TransformCoord(&offset, &offset, &rot);

    return center + offset;
}

AttackShort* Player::GetShortAttackState() const
{
    // m_pAttackManager は std::unique_ptr なので get() で生のポインタを取得
    if (m_pAttackManager)
    {
        // PlayerAttackManager の新しいメンバ関数を呼び出す
        return m_pAttackManager->GetCurrentShortAttack();
    }
    return nullptr;
}

void Player::SetCaptureState(float duration)
{
    PlayerState* nextState = m_pPlayerAnim.get();
    if (m_pCurrentState == nextState)
    {
        return;
    }

    // タイマーをセット
    m_CaptureTimer = duration;

    // State 遷移ロジック
    if (m_pCurrentState) {
        m_pCurrentState->Exit();    // 古いステートの終了処理
    }
    m_pCurrentState = nextState;    // ステートを切り替え
    if (m_pCurrentState) {
        m_pCurrentState->Enter();   // 新しいステートの開始処理（PortalAnim::Enter() が呼ばれる）
    }
}

bool Player::IsCapturingState() const
{
    return m_pCurrentState == m_pPlayerAnim.get();
}
