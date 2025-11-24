#pragma once
#include "..//NomalState.h"	//基底クラス.
#include <d3dx9.h> 

class EnemyNomal;
class NomalContext;

/********************************************************************
*	NomalMoveClass
**/
class NomalMove final
	: public NomalState
{
public:
	// ★追加: デフォルトコンストラクタ (エラー対策)
	NomalMove();

	NomalMove(EnemyNomal* pOwner);
	~NomalMove();

	void Enter() override;
	void Update() override;
	void Exit() override;

	void Draw() override;
	void Init() override;

public:
	const D3DXVECTOR3& GetDirectionToPlayer() const {
		return m_DirectionToPlayer;
	}
private:
	// ★EnemyNomal から移動してきたメンバ変数★
	float m_RotationSpeed;
	float m_RotationDirection;
	float m_RotationAngle;
	D3DXVECTOR3 m_DirectionToPlayer;

	// 移動ロジックで使用する定数
	static constexpr float MIN_DISTANCE_SQ = 0.01f * 0.01f;
	static constexpr float ENEMY_NOMAL_RADIUS = 10.0f;
	static constexpr float SPECIFIED_RANGE_ANGLE = D3DX_PI / 4.0f;
	static constexpr float MOVE_SPEED = 3.0f;
};