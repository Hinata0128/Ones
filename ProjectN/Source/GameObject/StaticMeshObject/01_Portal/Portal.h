#pragma once
#include "GameObject//StaticMeshObject//StaticMeshObject.h"	//基底クラス.

class StaticMeshManager;
class ImGuiManager;
class Player;
class Boss;

class PortalGauge;

/************************************************************
*	陣地を守用のポータル
*	取り合いのゲームだから1個で作成していく.
*	表示をするのが目的なのでUpdate()を回さない.
**/

class Portal final
	: public StaticMeshObject
{
public:
	//列挙.
	enum class PortalPriority : byte
	{
		None,		//誰も触っていない開幕の状態.
		Player,		//Playerがポータルを触った時.
		Enemy		//Enemyがポータルを触った時.
	};
public:
	Portal();
	~Portal() override;

	void Update() override;
	void Draw() override;
	//初期化関数.
	void Init();

	//タイムアップ時に強制的に決着をつける関数.
	void ForceFinishByTimeUp();
public:
	//===============================================================================
	// ポータルを触るときにPlayerとEnemyNomalの位置が必要なので
	// GameMainClassにあるm_pPlayerとm_pEnemyNomalの座標を取得するように
	// PortalClassにSet関数を作成しました.
	//===============================================================================
	//Playerの位置を設定するSet関数.
	void SetPlayer(std::shared_ptr<Player> player) { m_pPlayer = player; }
	//敵の位置を設定するSet関数.
	void SetEnemyNomal(std::shared_ptr<Boss> nomal) { m_pEnemy = nomal; }

	//ポータルのステートのSet関数.
	void SetPortalState(PortalPriority state);

	//ポータルの優先順位を入手する関数.
	void ChackPriority();

	int GetPortalPercent() const { return m_PortalIncrease; }

	//敵のAIでのだれがポータルを取得しているか分かるようにするGet関数.
	PortalPriority GetPortalState() const {
		return m_pPortalState;
	}

	// UIをセットする関数を追加
	void SetPortalGauge(std::shared_ptr<PortalGauge> gauge) { m_spPortalGauge = gauge; }

	//アクセス違反対策関数.
	//ToDo : 制限時間よりも先にポータルを100%にすると
	//		 アクセス違反になるのでそれの対策.
	bool IsTransitionStarted() const { return m_IsTransitionStarted; }

	// 遷移の準備ができたか
	bool IsReadyToLoad() const { return m_IsReadyToLoad; }
	// 次のシーンを取得（循環参照を避けるためintで返す）
	int GetNextScene() const { return m_NextScene; }


private:
	//Playerのポータル周りのコード.
	void PlayerToPortal();
	//EnemyNomalのポータル周りのコード.
	void EnemyToPortal();
	//先に範囲に入った陣営を記録.
	PortalPriority m_FirstEnterPriority = PortalPriority::None;
private:
	//ポータルの範囲に入った時にパーセントを増やす.
	float m_PortalIncreaseF;
	int m_PortalIncrease;

	//Playerの位置の取得のためにメンバ変数としてしようする.
	std::weak_ptr<Player> m_pPlayer;
	//敵の位置の取得のためにメンバ変数として使用します
	std::weak_ptr<Boss> m_pEnemy;

	//ポータルの行動列挙.
	PortalPriority m_pPortalState;

	std::shared_ptr<PortalGauge> m_spPortalGauge;

	//======================================================
	// ポータル周りに必要になるメンバ変数.
	//======================================================

	float m_IsPlayerPriority;
	float m_IsEnemyPriority;

	bool m_IsRoundFinished = false;

	float m_TransitionTimer = 0.0f;      // 遷移用のタイマー
	const float WAIT_TIME = 2.0f;        // 待機時間（秒）例：2.0秒
	bool m_IsTransitionStarted = false;  // 遷移プロセスが始まったかどうかのフラグ

	int m_NextSceneID = -1;

	bool m_IsReadyToLoad = false;

	int m_NextScene = -1; // -1 は遷移なし

};