#include "NomalMove.h"

#include "..//..//NomalContext/NomalContext.h"
#include "..//..//..//..//..//..//..//System/02_Singleton/Timer/Timer.h"
#include "..//..//EnemyNomal.h" 
#include <algorithm> // std::atan2 を使うため

// pOwner を取るコンストラクタ
NomalMove::NomalMove(EnemyNomal* pOwner)
	: NomalState(pOwner)
	, m_RotationSpeed(0.2f)
	, m_RotationDirection(1.0f)
	, m_RotationAngle(0.0f)
	, m_DirectionToPlayer(0.0f, 0.0f, 0.0f)
{
}

NomalMove::~NomalMove()
{
}

void NomalMove::Enter()
{
	NomalState::Enter();
}

void NomalMove::Update()
{
	NomalState::Update();

	EnemyNomal* pEnemy = dynamic_cast<EnemyNomal*>(m_pOwner);
	if (!pEnemy) return;

	float deltaTime = Timer::GetInstance().DeltaTime();

	const D3DXVECTOR3& EnemyPos = pEnemy->GetPosition();
	const D3DXVECTOR3& PlayerPos = pEnemy->GetPlayerPos();

	// 2. デルタタイムを使用して回転角度を更新させる
	float deltaAngle = m_RotationSpeed * deltaTime * m_RotationDirection;
	m_RotationAngle += deltaAngle;

	// 3. 角度が半径の範囲を超えたら回転方向を反転させる
	if (abs(m_RotationAngle) >= SPECIFIED_RANGE_ANGLE)
	{
		m_RotationDirection *= -1.0f;
	}

	// 4. 回転行列でオフセットベクトルを計算
	D3DXMATRIX RotatedMatrix;
	D3DXMatrixRotationY(&RotatedMatrix, m_RotationAngle);

	D3DXVECTOR3 InitialVec(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 RotatedVec;
	D3DXVec3TransformNormal(&RotatedVec, &InitialVec, &RotatedMatrix);
	D3DXVec3Normalize(&RotatedVec, &RotatedVec);

	// 5. 敵の向きを計算し、m_DirectionToPlayer と敵の Y 軸回転を更新
	D3DXVECTOR3 Direction;
	D3DXVec3Subtract(&Direction, &PlayerPos, &EnemyPos);
	D3DXVec3Normalize(&m_DirectionToPlayer, &Direction);

	float dx = Direction.x;
	float dz = Direction.z;
	float Angle_Radian = std::atan2(dx, dz) + D3DX_PI;
	pEnemy->SetRotationY(Angle_Radian);

	// ------------------------------------
	// 敵の位置を円軌道状に更新する処理.
	// ------------------------------------

	// オフセットベクトルを計算 
	D3DXVECTOR3 offsetVec;
	D3DXVec3Scale(&offsetVec, &RotatedVec, ENEMY_NOMAL_RADIUS);

	// 新しい位置を計算: プレイヤーの位置 + オフセット (理想的なターゲット位置)
	D3DXVECTOR3 newEnemyPos;
	D3DXVec3Add(&newEnemyPos, &PlayerPos, &offsetVec);

	//=========================================================
	// 理想的な円軌道上のターゲット位置へゆっくりと移動する.
	//=========================================================

	// 理想位置までの差分ベクトルを計算
	D3DXVECTOR3 MoveDirection;
	D3DXVec3Subtract(&MoveDirection, &newEnemyPos, &EnemyPos);

	// 距離チェック
	float DistanceSq = D3DXVec3LengthSq(&MoveDirection);

	if (DistanceSq > MIN_DISTANCE_SQ)
	{
		// 移動ベクトルを正規化
		D3DXVec3Normalize(&MoveDirection, &MoveDirection);

		// 1フレームでの移動量を計算
		D3DXVECTOR3 MoveStep;
		D3DXVec3Scale(&MoveStep, &MoveDirection, MOVE_SPEED * deltaTime);

		// 現在の位置に移動量を加算
		pEnemy->AddPosition(MoveStep);
	}
}

void NomalMove::Exit()
{
	NomalState::Exit();
}

void NomalMove::Draw()
{
	NomalState::Draw();
}

void NomalMove::Init()
{
	NomalState::Init();
}