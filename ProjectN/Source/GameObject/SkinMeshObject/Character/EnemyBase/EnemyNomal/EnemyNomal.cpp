#include "EnemyNomal.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "System/00_Manager/04_EnemyNomalShotManager/EnemyNomalShotManager.h"
#include "System/02_Singleton/Timer//Timer.h"

// Time.h が D3DX を使用している前提で、Timer::GetInstance().DeltaTime() を使用
// BossMoveState のコードでは Time::GetInstance().GetDeltaTime() を使用していましたが、
// EnemyNomal の既存のコードに合わせて Timer::GetInstance().DeltaTime() を使用します。


constexpr float zero = 0.0f;

EnemyNomal::EnemyNomal()
    : EnemyBase()
    , m_pENShotManager(nullptr)

    , m_RotationSpeed(0.2f)
    , m_RotationDirection(1.0f)
    , m_RotationAngle(0.0f)
    , m_ApproachSpeed(1.5f)
    , m_DirectionToPlayer(0.0f, 0.0f, 0.0f)
{
    SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Enemy);
    auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh, [](SkinMesh*) {});
    AttachMesh(shared_mesh);
    //サイズの変更.
    SetScale(D3DXVECTOR3(0.004f, 0.004f, 0.004f));
    SetPosition(0.0f, 0.0f, 10.0f);

    m_AnimSpeed = 0.0002f;

    m_pENShotManager = EnemyNomalShotManager::GetInstance();

    m_BSphere.SetRadius(0.5f);

    //当たり判定の位置を変更.
    m_HitCenterOffset = D3DXVECTOR3(zero, 3.0f, zero);

    m_ShotOffset = D3DXVECTOR3(zero, 1.5f, zero);

    // クールタイム初期化
    // 最初のフレームで発射されないように、m_CoolTimeと同じ値で初期化する
    m_CoolTime = 1.0f; // 1秒ごとに発射
    m_ShotCoolDown = m_CoolTime; 
}

EnemyNomal::~EnemyNomal()
{
}

void EnemyNomal::Update()
{
    m_pMesh->SetAnimSpeed(m_AnimSpeed);
    //デルタタイムを取得する.
    float deltaTime = Timer::GetInstance().DeltaTime();

    // クールタイム減算
    m_ShotCoolDown -= deltaTime;
    if (m_ShotCoolDown < 0.0f) m_ShotCoolDown = 0.0f;

    // ボーン座標取得
    m_pMesh->GetPosFromBone("entotu_Joint", &m_BonePos);

    // アニメーション更新
    m_pAnimCtrl->AdvanceTime(m_AnimSpeed, nullptr);

    // AutoShot() は m_DirectionToPlayer の更新後であるべきだが、
    // コンストラクタでのクールタイム修正により、最初の発射は遅延されるため、
    // ここにAutoShotがあっても、2回目以降は正しく動作する。
    AutoShot();

    //=========================================================
    //円運動のロジック (ゆっくり追従するように修正).
    //=========================================================
    {
        // 弾の更新
        //m_pENShotManager->Update();

        // 現在の位置とターゲット（プレイヤー）の位置を取得
        const D3DXVECTOR3& EnemyPos = GetPosition();
        const D3DXVECTOR3& PlayerPos = m_pPlayerPos;

        // デルタタイムを使用して回転角度を更新させる.
        float deltaAngle = m_RotationSpeed * deltaTime * m_RotationDirection;

        // 回転角度のメンバ変数を更新.
        m_RotationAngle += deltaAngle;

        // 角度が半径の範囲を超えたら回転方向を反転させる.
        const float Specified_Range_Angle = D3DX_PI / 4.0f;
        if (abs(m_RotationAngle) >= Specified_Range_Angle)
        {
            // 方向の反転.
            m_RotationDirection *= -1.0f;
        }

        // Y軸周りの回転行列で変換.
        D3DXMATRIX RotatedMatrix;
        D3DXMatrixRotationY(&RotatedMatrix, m_RotationAngle);

        // ベクトルを回転行列で変換. (基準ベクトル: Z軸方向)
        D3DXVECTOR3 InitialVec(0.0f, 0.0f, 1.0f);
        D3DXVECTOR3 RotatedVec;
        D3DXVec3TransformNormal(&RotatedVec, &InitialVec, &RotatedMatrix);

        // 正規化.
        D3DXVec3Normalize(&RotatedVec, &RotatedVec);

        constexpr float EnemyNomal_Radius = 10.0f;

        // 敵をプレイヤーの方向に向かせる. (向きの計算)
        D3DXVECTOR3 Direction;
        D3DXVec3Subtract(&Direction, &PlayerPos, &EnemyPos);

        // ★ここで m_DirectionToPlayer が初めて正しい値に更新される★
        D3DXVec3Normalize(&m_DirectionToPlayer, &Direction);

        // X,Z平面での向きを取得.
        float dx = Direction.x;
        float dz = Direction.z;

        // atan2fでY軸回転角度を計算.
        float Angle_Radian = std::atan2(dx, dz);

        Angle_Radian += D3DX_PI;

        SetRotationY(Angle_Radian); // 敵の回転をセット


        // ------------------------------------
        // 敵の位置を円軌道状に更新する処理.
        // ------------------------------------

        // (1) オフセットベクトルを計算 (回転ベクトル * 半径)
        D3DXVECTOR3 offsetVec;
        D3DXVec3Scale(&offsetVec, &RotatedVec, EnemyNomal_Radius);

        // (2) 新しい位置を計算: プレイヤーの位置 + オフセット (円軌道上の【理想的なターゲット位置】)
        D3DXVECTOR3 newEnemyPos;
        D3DXVec3Add(&newEnemyPos, &PlayerPos, &offsetVec);

        //=========================================================
        //理想的な円軌道上のターゲット位置へゆっくりと移動する.
        //=========================================================

        // (3) 理想位置 (newEnemyPos) までの差分ベクトルを計算
        D3DXVECTOR3 MoveDirection;
        D3DXVec3Subtract(&MoveDirection, &newEnemyPos, &EnemyPos);

        // (4) 現在の敵と理想位置との距離をチェック (近づきすぎた際の微振動防止)
        float DistanceSq = D3DXVec3LengthSq(&MoveDirection);
        constexpr float MIN_DISTANCE_SQ = 0.01f * 0.01f; // 0.01^2 = 0.0001 (1cm未満なら移動しない)

        // m_ApproachSpeed がクラスに定義されていることを前提とする
        // 必要に応じて、ここで速度を直接定義しても良い: float MoveSpeed = 3.0f;
        const float MoveSpeed = 3.0f; // 追従速度 (調整してください)

        if (DistanceSq > MIN_DISTANCE_SQ)
        {
            // (5) 移動ベクトルを正規化
            D3DXVec3Normalize(&MoveDirection, &MoveDirection);

            // (6) デルタタイムと速度を乗算し、1フレームでの移動量を計算
            D3DXVECTOR3 MoveStep;
            D3DXVec3Scale(&MoveStep, &MoveDirection, MoveSpeed * deltaTime);

            // (7) 現在の位置に移動量を加算し、ゆっくりと理想位置に追従させる
            AddPosition(MoveStep);
        }

        // SetPosition(newEnemyPos); // ★元の瞬間移動ロジックは削除★
    }
    // =========================================================


    EnemyBase::Update();
}

void EnemyNomal::Draw()
{
    m_pMesh->SetAnimSpeed(m_AnimSpeed);
    EnemyBase::Draw();
   // m_pENShotManager->Draw();
}

void EnemyNomal::Init()
{
    EnemyBase::Init();
}

// 弾の飛ぶ方向（常に -Z）
D3DXVECTOR3 EnemyNomal::Enemy_WS() const
{
    D3DXVECTOR3 dir;

    dir.x = sinf(m_vRotation.y);
    dir.y = zero;
    dir.z = -cosf(m_vRotation.y);
    return dir;
}

void EnemyNomal::AutoShot()
{
    // 弾の発射（自動）
    if (m_ShotCoolDown == 0.0f)
    {
        // 変換行列作成
        D3DXMATRIX matS, matR, matT, enemyWorldMatrix;
        D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
        D3DXMatrixRotationYawPitchRoll(&matR, m_vRotation.y, m_vRotation.x, m_vRotation.z);
        D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
        D3DXMatrixMultiply(&enemyWorldMatrix, &matS, &matR);
        D3DXMatrixMultiply(&enemyWorldMatrix, &enemyWorldMatrix, &matT);

        // ボーン座標をワールド座標に変換
        D3DXVECTOR3 worldBonePos;
        D3DXVec3TransformCoord(&worldBonePos, &m_BonePos, &enemyWorldMatrix);

        D3DXVECTOR3 shotPos = worldBonePos + m_ShotOffset;

        // 弾の発射方向は、m_DirectionToPlayerを使用
        D3DXVECTOR3 Dir = m_DirectionToPlayer;
        // 弾を追加
        m_pENShotManager->AddEnemyNomalShot(shotPos, Dir);

        // クールタイムリセット
        m_ShotCoolDown = m_CoolTime;
    }
}

D3DXVECTOR3 EnemyNomal::GetHitCenter() const
{
    // プレイヤーモデルの位置 (m_vPosition) にオフセットを加算して返す
    return m_vPosition + m_HitCenterOffset;
}