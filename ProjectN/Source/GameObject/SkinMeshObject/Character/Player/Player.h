#pragma once
#include "GameObject/SkinMeshObject/Character/Character.h"

class PShotManager;
class Timer;
class PlayerState;
class PlayerIdol;
class PlayerContext;
class PlayerDead;

//ポータル取得時に再生するアニメーションのクラス.
class PlayerPortalAnim;

#include "System/00_Manager/07_PlayerAttackManager/PlayerAttackManager.h"
#include "..//Player/PlayerAttckStateBase/AttackShort/AttackShort.h"

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

public:

	BoundingSphere& GetBoundingSphere() { return m_BSphere; }

	PShotManager* GetShotManager() { return m_pShotManager; }

	D3DXVECTOR3 GetHitCenter() const;

	//HPを取得する関数.
	float GetHitPoint() const { return m_HitPoint; }
	// Playerの動作の初期化用関数.
	// 攻撃マネージャークラスの遠距離攻撃の初期化.
	// PlayerIdolの初期化.
	// 他の方法で作成するやり方を調べて実装する.
	void InitializePlayerMove();

public:
	//Playerの動作用関数.
	//W・Sの前進後退用関数.
	D3DXVECTOR3 Player_WS(float RotationY) const;
	//A・Dの左右用関数.
	D3DXVECTOR3 Player_AD(float RotationY) const;

	void ChangeAttackType(PlayerAttackManager::enAttack type);
	void CleanUpAttackState(PlayerAttackManager::enAttack type);

	// ボーンの位置を取得する関数
	bool GetBonePosition(const char* boneName, D3DXVECTOR3* outPos) const;

	D3DXVECTOR3 GetShortAttackCenter() const;
	AttackShort* GetShortAttackState() const;

	PlayerAttackManager* GetAttackManager() const { return m_pAttackManager.get(); }

public:
	//ポータル系の関数をここに書く.
	//PortalからStateを切り替えるため.
	//セットキャプチャステート関数.
	void SetCaptureState(float duration);
	//Portalがゲージ増加停止を判定するための関数.
	bool IsCapturingState() const;
private:
	PShotManager*	m_pShotManager;
	std::unique_ptr<PlayerAttackManager> m_pAttackManager;

	BoundingSphere m_BSphere; //プレイヤー用バウンディングスフィア.

	std::shared_ptr<PlayerIdol>		m_pPlayerIdol;
	std::shared_ptr<PlayerDead>		m_pPlayerDead;

	std::shared_ptr<PlayerPortalAnim> m_pPlayerAnim;

	PlayerState* m_pCurrentState;

	float m_CaptureTimer = 0.0f;
};