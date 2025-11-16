#pragma once
#include "GameObject/SkinMeshObject/Character/Character.h"

class PShotManager;
class Timer;
class PlayerState;
class PlayerIdol;
class PlayerContext;
class PlayerDead;
//class PlayerAttackManager;
#include "System/00_Manager/07_PlayerAttackManager/PlayerAttackManager.h"


/**************************************************
*	プレイヤークラス.
**/
class Player final
	: public Character
{
public:
	friend class PlayerContext;
public:
	Player();
	 ~Player() override;

	void Update() override;
	void Draw() override;
	void Init() override;

	void Hit();

	void SetAnimSpeed(double speed) { m_AnimSpeed = speed; }

	void Stop();

public:

	BoundingSphere& GetBoundingSphere() { return m_BSphere; }

	PShotManager* GetShotManager() { return m_pShotManager; }

	D3DXVECTOR3 GetHitCenter() const;

	//HPを取得する関数.
	float GetHitPoint() const { return m_HitPoint; }

public:
	//Playerの動作用関数.
	//W・Sの前進後退用関数.
	D3DXVECTOR3 Player_WS(float RotationY) const;
	//A・Dの左右用関数.
	D3DXVECTOR3 Player_AD(float RotationY) const;

	void ChangeAttackType(PlayerAttackManager::enAttack type);
private:
	PShotManager*	m_pShotManager;
	std::unique_ptr<PlayerAttackManager> m_pAttackManager;

	BoundingSphere m_BSphere; //プレイヤー用バウンディングスフィア.

	std::shared_ptr<PlayerIdol>		m_pPlayerIdol;
	std::shared_ptr<PlayerDead>		m_pPlayerDead;

	PlayerState* m_pCurrentState;
};