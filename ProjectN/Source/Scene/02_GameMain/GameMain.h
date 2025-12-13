#pragma once

//ここも継承させているわけではないのでSceneBaseだけインクルードして
//他は前方宣言をしておく.
#include "Scene//SceneBase.h"
#include "GameObject//SpriteObject//SpriteObject.h"
#include "SceneManager//SceneManager.h"

#include "GameObject//StaticMeshObject//Ground//Ground.h"
#include "GameObject//SkinMeshObject//Character//Player//Player.h"

#include "GameObject/SkinMeshObject/Character/EnemyBase/00_Boss/Boss.h"

#include "System/00_Manager/02_PShotManager/PShotManager.h"
#include "System/00_Manager/04_BossShotManager/BossShotManager.h"

#include "System/00_Manager/06_CollisionManager/CollisionManager.h"

#include "Sprite3D//Sprite3D.h"

#include "GameObject//StaticMeshObject//Portal//Portal.h"

#include "GameObject//UIObject//00_HPBar//HPBar.h"

class Timer;


/*********************************************
*	ゲームメイン画像クラス.
**/

class GameMain
	: public SceneBase
{
public:
	GameMain();
	~GameMain() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	HRESULT LoadData();

	//カメラ関数.
	void Camera();
	//プロジェクション関数.
	void Projection();

	void UpdateCamera();

private:
	//ゲームで扱うスプライトデータ(使いまわす資源).
	std::unique_ptr<Sprite3D> m_pSp3D;

	//スタティックメッシュオブジェクトクラス.
	std::unique_ptr<StaticMeshObject> m_pStcMeshObj;

	//地面クラス.
	std::unique_ptr<Ground>	m_pGround;

	//プレイヤークラス.
	std::shared_ptr<Player> m_pPlayer;

	std::shared_ptr<Boss> m_pEnemyNomal;

	std::shared_ptr<CollisionManager> m_pCollisionManager;

	//ポータルクラス.
	std::unique_ptr<Portal> m_pPortal;

	//HPBarクラス(Test).
	std::unique_ptr<HPBar> m_pHpBar;

};