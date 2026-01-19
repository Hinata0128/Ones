#pragma once
#include "GameObject/00_SkinMeshObject/Character/Character.h"

//メッシュをアタッチする.
class SkinMeshManager;
//プレイヤーの弾マネージャークラス.
class PShotManager;
class Timer;
//プレイヤーのステートクラス.
class PlayerState;
//待機クラス.
class PlayerIdol;
//アニメーションの設を呼ぶクラス.
class PlayerContext;
//死んだとき.
class PlayerDead;

//ポータル取得時に再生するアニメーションのクラス.
class PlayerPortalAnim;
class Shadow;

#include "System/00_Manager/07_PlayerAttackManager/PlayerAttackManager.h"
#include "..//01_Player/PlayerAttckStateBase/AttackShort/AttackShort.h"


/**************************************************
*	プレイヤークラス.
*	ToDo : Characterクラスを親として継承関係を結ぶ.
**/
class Player final
	: public Character
{
public:
	//フレンドを結んでいる.
	friend class PlayerContext;
public:
	Player();
	 ~Player() override;

	void Update() override;
	void Draw() override;
	void Init() override;

	void Hit();
public:

	BoundingSphere& GetBoundingSphere() { return m_BSphere; }

	PShotManager* GetShotManager() { return m_pShotManager; }
	//当たり判定の位置を設定.
	D3DXVECTOR3 GetHitCenter() const;

	//HPを取得する関数.
	float GetHitPoint() const { return m_HitPoint; }
	// Playerの動作の初期化用関数.
	// 攻撃マネージャークラスの遠距離攻撃の初期化.
	// PlayerIdolの初期化.
	void InitializePlayerMove();
	
	//ToDo : プレイヤーが死亡した際に呼ばれる関数.
	//復活関数.
	void Respawn();
	//死んだ状態取得関数.
	bool IsDead() const { return m_pCurrentState == (PlayerState*)m_pPlayerDead.get(); }

public:
	//Playerの動作用関数.
	//W・Sの前進後退用関数.
	D3DXVECTOR3 Player_WS(float RotationY) const;
	//A・Dの左右用関数.
	D3DXVECTOR3 Player_AD(float RotationY) const;

	//攻撃ステートの切り替え関数.
	//ToDo : PlayerAttackManagerに遷移タイミングは任している.
	void ChangeAttackType(PlayerAttackManager::enAttack type);

	// ボーンの位置を取得する関数
	bool GetBonePosition(const char* boneName, D3DXVECTOR3* outPos) const;

	//プレイヤーの遠距離攻撃関数.
	//ToDo : 遠距離攻撃を発射する座標を設定している.
	D3DXVECTOR3 GetShortAttackCenter() const;
	//.
	AttackShort* GetShortAttackState() const;

	PlayerAttackManager* GetAttackManager() const { return m_pAttackManager.get(); }
public:
	//PortalからStateを切り替えるため.
	//セットキャプチャステート関数.
	void SetCaptureState(float duration);
	//Portalがゲージ増加停止を判定するための関数.
	bool IsCapturingState() const;

	Shadow* GetShadow() const		{ return m_pShadow.get(); }

	void SetVisible(bool visible)	{ m_IsVisible = visible; }
	bool IsVisible() const			{ return m_IsVisible; }

	void SetFrozen(bool frozen)		{ m_IsFrozen = frozen; }
	bool IsFrozen() const			{ return m_IsFrozen; }
private:
	PShotManager*	m_pShotManager;
	std::unique_ptr<PlayerAttackManager> m_pAttackManager;

	BoundingSphere m_BSphere; //プレイヤー用バウンディングスフィア.

	std::shared_ptr<PlayerIdol>		m_pPlayerIdol;
	std::shared_ptr<PlayerDead>		m_pPlayerDead;

	std::shared_ptr<PlayerPortalAnim> m_pPlayerAnim;

	PlayerState* m_pCurrentState;

	float m_CaptureTimer = 0.0f;

	D3DXVECTOR3 m_InitialPosition;

	std::unique_ptr<Shadow> m_pShadow;

	bool m_IsVisible = true;

	bool m_IsFrozen;
};