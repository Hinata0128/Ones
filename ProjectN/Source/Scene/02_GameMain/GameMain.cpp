#include "GameMain.h"
#include "Sound//SoundManager.h"
#include "Effect//Effect.h"

#include "System//02_Singleton//Timer//Timer.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"

#include "System/00_Manager/02_PShotManager/PShotManager.h"
#include "System/00_Manager/04_BossShotManager/BossShotManager.h"
#include "System/00_Manager/06_CollisionManager/CollisionManager.h" 
#include "GameObject//SkinMeshObject//Character//Player//Player.h" 

GameMain::GameMain()
	: SceneBase()
	, m_pStcMeshObj(std::make_unique<StaticMeshObject>())

	, m_pGround(std::make_unique<Ground>())

	, m_pPlayer(std::make_shared<Player>())

	, m_pPortal(std::make_unique<Portal>())

	, m_pEnemyNomal(std::make_shared<Boss>(m_pPortal))

	, m_pCollisionManager(std::make_shared<CollisionManager>())

	, m_pHpBar(std::make_unique<HPBar>())

	, m_pSkyBox(std::make_unique<BackGround>())

	, m_pPointUI(std::make_unique<Point>())

	, m_pPortalGauge(std::make_shared<PortalGauge>())
{
	m_pDx11 = DirectX11::GetInstance();
	m_pDx9 = DirectX9::GetInstance();

	//カメラの位置を変更できるところ.
	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	//カメラを見ているところを変更できるところ.
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);

	//ライト情報
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);		//ライト方向.
	Initialize();
}

GameMain::~GameMain()
{
	//外部で作成しているので、ここでは破棄しない.
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

void GameMain::Initialize()
{
	//カメラの位置を初期化.
	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);
}

void GameMain::Create()
{
	//これでGameMainにあるPlayerの位置を取得してポータルの増加を作成している.
	m_pPortal->SetPlayer(m_pPlayer);
	//これでGameMainにあるEnemyNomalの位置を取得してポータルの増加を作成している.
	m_pPortal->SetEnemyNomal(m_pEnemyNomal);

	m_pPortal->SetPortalGauge(m_pPortalGauge);

	m_pHpBar->SetTargetPlayer(m_pPlayer.get());
	m_pHpBar->Create();

	m_pPointUI->Create();
	m_pPortalGauge->Create();

	BossShotManager::GetInstance()->Init();
	PShotManager::GetInstance()->Init();

	//ポータルの初期化.
	m_pPortal->Init();

	UpdateCamera();
}

void GameMain::Update()
{
	//Timer更新
	Timer::GetInstance().Update();

	//Effect制御 (省略)
	{
		//エフェクトのインスタンスごとに必要なハンドル
		//※３つ表示して制御するなら３つ必要になる
		static ::EsHandle hEffect = -1;

		if (GetAsyncKeyState('Y') & 0x0001) {
			hEffect = Effect::Play(Effect::Test0, D3DXVECTOR3(0.f, 1.f, 0.f));

			//拡大縮小
			Effect::SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));

			//回転(Y軸回転)
			Effect::SetRotation(hEffect, D3DXVECTOR3(0.f, D3DXToRadian(90.0f), 0.f));

			//位置を再設定
			Effect::SetLocation(hEffect, D3DXVECTOR3(0.f, 1.f, 1.f));
		}
		if (GetAsyncKeyState('T') & 0x0001) {
			Effect::Stop(hEffect);
		}
	}

	//地面.
	m_pGround->Update();
	m_pPlayer->Update();
	UpdateCamera();

	{
		m_pEnemyNomal->Update();

		m_pEnemyNomal->SetTargetPos(m_pPlayer->GetPosition());
	}

	// =========================================================================


	auto playerShotMgr = PShotManager::GetInstance();
	auto enemyShotMgr = BossShotManager::GetInstance();


	m_pCollisionManager->SetPlayer(m_pPlayer); // shared_ptr を渡す
	m_pCollisionManager->SetEnemies({m_pEnemyNomal.get()}); // EnemyNomalManagerにGetEnemies()が必要です
	m_pCollisionManager->SetPlayerShots(playerShotMgr->GetShots()); // PShotManagerにGetShots()が必要です
	m_pCollisionManager->SetEnemyShots(enemyShotMgr->GetShots()); // EnemyNomalShotManagerにGetShots()が必要です

	m_pCollisionManager->Update();


	m_pHpBar->Update();

	// ==========================================================
	// 【修正】SceneManagerから「累計スコア」を取得してUIに送る
	// ==========================================================
	// SceneManagerに GetPlayerScore() という関数がある前提です。
	// もし名前が違ったら、実際の関数名に合わせてください。
	int totalPlayerScore = SceneManager::GetInstance()->GetPlayerScore();

	// 累計スコアが1以上なら1つ目を点灯
	if (totalPlayerScore >= 1) {
		m_pPointUI->SetPlayerPointActive(0, true);
	}
	// 累計スコアが2以上なら2つ目を点灯
	if (totalPlayerScore >= 2) {
		m_pPointUI->SetPlayerPointActive(1, true);
	}

	// 敵のスコアも同様に反映させる場合
	int totalEnemyScore = SceneManager::GetInstance()->GetEnemyScore();
	// if (totalEnemyScore >= 1) m_pPointUI->SetEnemyPointActive(0, true);
	// ==========================================================

		// 累計スコアが1以上なら1つ目を点灯
	if (totalEnemyScore >= 1) {
		m_pPointUI->SetBossPointActive(0, true);
	}
	// 累計スコアが2以上なら2つ目を点灯
	if (totalEnemyScore >= 2) {
		m_pPointUI->SetBossPointActive(1, true);
	}


	m_pPointUI->Update();

	// ポータルの更新（ここで100%判定とシーン遷移が行われる）
	m_pPortal->Update();


	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::First);
	}

	// 0x8000 を使うことで、キーが「押されている状態」を検知します
	if (GetAsyncKeyState('P') & 0x8000) {
		m_pPointUI->SetPlayerPointActive(0, true);
	}
}

void GameMain::Draw()
{
	Camera();
	Projection();

	PreDraw();

	//背景の表示.
	m_pSkyBox->Draw();

	//地面表示.
	m_pGround->Draw();
	m_pPlayer->Draw();

	m_pPortal->Draw();

	PShotManager::GetInstance()->Draw();

	m_pEnemyNomal->Draw();


#ifdef _DEBUG

	// バウンディングスフィアの描画（デバッグ用）
	m_pPlayer->GetBoundingSphere().Draw();

	m_pEnemyNomal->GetBoundingSphere().Draw();

	ImGui::Begin(JAPANESE("タイマー情報"));
	ImGui::Text(JAPANESE("デルタタイム: %.4f 秒"), Timer::GetInstance().DeltaTime());
	ImGui::Text(JAPANESE("総経過時間: %.2f 秒"), Timer::GetInstance().ElapsedTime());
	ImGui::End();
#endif
	//Effectクラス
	Effect::GetInstance()->Draw();
	m_pHpBar->Draw();
	m_pPointUI->Draw();
	m_pPortalGauge->Draw();

}

HRESULT GameMain::LoadData()
{

	return S_OK;
}

void GameMain::Camera()
{
	D3DXVECTOR3 cam_pos = m_Camera.vPosition;
	D3DXVECTOR3 cam_look = m_Camera.vLook;
	D3DXVECTOR3	vUpVec(0.0f, 1.0f, 0.0f);	//上方（ベクトル）.

	//ビュー（カメラ）変換.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&cam_pos, &cam_look, &vUpVec);
}

//プロジェクション関数.
void GameMain::Projection()
{
	//y方向の視野角。数値を大きくしたら視野が狭くなる.
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));	//ラジアン値.
	//アスペクト（幅÷高さ）.
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 999.9;

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		fov_y,		//視野角（FOV：Field of View）.
		aspect,		//アスペクト.
		near_z,		//近いビュー平面のz値.
		far_z);		//遠いビュー平面のz値.
}

//カメラをPlayerの背後に設定する.
void GameMain::UpdateCamera()
{
	//ローカル変数.
	constexpr float Ten = 5.0f;
	constexpr float Fifteen = 15.0f;

	D3DXVECTOR3 PlayerPos = m_pPlayer->GetPosition();
	//いったんカメラの位置をここに固定しています.
	m_Camera.vPosition = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + Ten, PlayerPos.z - Fifteen);
	m_Camera.vLook = PlayerPos;
}

