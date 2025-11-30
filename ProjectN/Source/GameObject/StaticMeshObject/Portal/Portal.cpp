#include "Portal.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"
#include "System/02_Singleton/Timer/Timer.h"
#include "GameObject/SkinMeshObject/Character/Player/Player.h"
#include "GameObject/SkinMeshObject/Character/EnemyBase/EnemyNomal/EnemyNomal.h"

#include "SceneManager/SceneManager.h"

Portal::Portal()
	: StaticMeshObject	()
	, m_PortalIncreaseF(0.0f)
	, m_PortalIncrease	(0)

	, m_pPortalState	(PortalPriority::None) 

	, m_IsPlayerPriority(false)
	, m_IsEnemyPriority(false)

{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Portal));

	//ポータルの位置を変更.
	D3DXVECTOR3 Pos		= { 0.0f, 0.0f, 5.0f };
	//ポータルのサイズ変更.
	D3DXVECTOR3 Scale	= { 1.5f, 1.5f, 1.5f };

	SetPosition(Pos);
	SetScale(D3DXVECTOR3(Scale));

	Init();
}

Portal::~Portal()
{
}

void Portal::Update()
{
	//============================================================================================================
	// ポータルの取得ゲージの増加をImGuiを使用して確認します.
	// 100%になった時に確認のため一回EndingであるWin画面に遷移させるようにする.
	//============================================================================================================

	ChackPriority();

	//enum classをしようしているのでswitchで作成する.
	switch (m_pPortalState)
	{
		//誰も触っていないとき.
	case Portal::PortalPriority::None:
		break;
		//Playerが触っているとき.
	case Portal::PortalPriority::Player:
		PlayerToPortal();
		break;
		//Enemyが触っているとき.
	case Portal::PortalPriority::Enemy:
		EnemyToPortal();
		break;
	default:
		break;
	}



	//時間経過での増加.
#if 0
	//経過時間を設定.
	static float ElapsedTime = 0.0f;
	//deltaTimeの取得.
	float deltaTime = Timer::GetInstance().DeltaTime();
	//経過時間に今の時間をたす.
	ElapsedTime += deltaTime;

	//もし一秒経過したら.
	if (ElapsedTime >= 1.0f)
	{
		ElapsedTime = 0.0f;
		m_PortalIncrease += 1;	//時間経過で取得可能な数値.

		if (m_PortalIncrease > 100)
		{
			m_PortalIncrease = 100;
		}
	}
#endif

#ifdef _DEBUG

	//表示するブロック.
	ImGui::Begin(JAPANESE("Portal : 増加量/占有者")); // ウィンドウタイトルを分かりやすく変更

	const char* owner_text;
	switch (m_pPortalState) {
	case PortalPriority::Player:
		owner_text = "Player (プレイヤー)";
		// プレイヤーが触っているときの色（例: 青）
		ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), JAPANESE("現在の占有者: %s"), owner_text);
		break;
	case PortalPriority::Enemy:
		owner_text = "Enemy (敵)";
		// 敵が触っているときの色（例: 赤）
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), JAPANESE("現在の占有者: %s"), owner_text);
		break;
	case PortalPriority::None:
	default:
		owner_text = "None (誰もいない/競合)";
		// 誰も触っていないときの色（例: 黄色）
		ImGui::TextColored(ImVec4(1.8f, 1.8f, 0.0f, 1.0f), JAPANESE("現在の占有者: %s"), owner_text);
		break;
	}

	//スライダー(手動で触れるようにしている).
	ImGui::SliderFloat(JAPANESE("Portalの取得パーセント(%)"), &m_PortalIncreaseF, 0.0f, 100.0f);
	//今進んでいるパーセンテージ.
	ImGui::Text(JAPANESE("今のポータルのパーセント : %d"), m_PortalIncrease);

	if (m_PortalIncrease >= 100)
	{
		//ポータルが100%になった時に表示してくれるもの.
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Portal Full");
	}

	ImGui::End();
#endif
	StaticMeshObject::Update();
}

void Portal::Draw()
{
	StaticMeshObject::Draw();
}

void Portal::Init()
{
	//ここにゲージの初期化を入れる.
	m_PortalIncreaseF = 0.0f;
	//列挙の状態を初期化させる.
	m_pPortalState = PortalPriority::None;
}

void Portal::SetPortalState(PortalPriority state)
{
	if (m_pPortalState == state)
	{
		return;
	}

	if (m_pPortalState == PortalPriority::None)
	{
		m_pPortalState = state;
		return;
	}

	m_pPortalState = state;
}

void Portal::ChackPriority()
{
	//ゲージの進む範囲.
	constexpr float PORTAL_DISTANCE = 5.0f;
	float playerDistance = 1000.0f;
	float enemyDistance = 1000.0f;  

	//Playerの距離チェックと m_IsPlayerPriority の更新
	m_IsPlayerPriority = false;
	if (auto player = m_pPlayer.lock())
	{
		D3DXVECTOR3 diff = player->GetPosition() - GetPosition();
		playerDistance = D3DXVec3Length(&diff); //距離を計算

		if (playerDistance <= PORTAL_DISTANCE)
		{
			m_IsPlayerPriority = true;
		}
	}

	//Enemyの距離チェックと m_IsEnemyPriority の更新
	m_IsEnemyPriority = false;
	if (auto enemy = m_pEnemy.lock())
	{
		D3DXVECTOR3 Diff = enemy->GetPosition() - GetPosition();
		enemyDistance = D3DXVec3Length(&Diff); //距離を計算

		if (enemyDistance <= PORTAL_DISTANCE)
		{
			m_IsEnemyPriority = true;
		}
	}

	// 💡 【修正】時間優先ロジックとアニメーション起動
	if (m_IsPlayerPriority && !m_IsEnemyPriority)
	{
		// Playerのみ範囲内: Playerに切り替え
		if (m_pPortalState != PortalPriority::Player)
		{
			// 💡 状態が切り替わる瞬間: Playerを最初の侵入者として記録し、アニメーションをトリガー
			if (auto player = m_pPlayer.lock())
			{
				player->SetCaptureState(1.0f); // 1.0秒間 State を固定
			}
			m_FirstEnterPriority = PortalPriority::Player;
		}
		m_pPortalState = PortalPriority::Player;
	}
	else if (!m_IsPlayerPriority && m_IsEnemyPriority)
	{
		// Enemyのみ範囲内: Enemyに切り替え
		if (m_pPortalState != PortalPriority::Enemy)
		{
			// 💡 状態が切り替わる瞬間: Enemyを最初の侵入者として記録し、アニメーションをトリガー
			if (auto enemy = m_pEnemy.lock())
			{
				// ここで Enemy の SetCaptureState() を呼び出します
				// enemy->SetCaptureState(1.0f); 
			}
			m_FirstEnterPriority = PortalPriority::Enemy;
		}
		m_pPortalState = PortalPriority::Enemy;
	}
	else if (m_IsPlayerPriority && m_IsEnemyPriority)
	{
		// 💡 【修正】競合時: 距離ではなく、先に範囲に入っていた人を優先する
		m_pPortalState = m_FirstEnterPriority;
	}
	else
	{
		// 誰もいない時は、前回の状態を維持（m_pPortalState は変更しない）
	}
}
//Playerのポータル周りのコード.
void Portal::PlayerToPortal()
{
	if (auto player = m_pPlayer.lock())
	{
		// 💡 【追加】Player がキャプチャ State 中はゲージ増加を停止
		if (player->IsCapturingState())
		{
			return; // ゲージ増加処理をスキップ
		}
	}

	float deltaTime = Timer::GetInstance().DeltaTime();

	m_PortalIncreaseF += deltaTime * 20.0f; // 💡 ゲージ速度を調整（以前の会話で20.0fを指定）

	m_PortalIncrease = static_cast<int>(m_PortalIncreaseF);

	if (m_PortalIncreaseF >= 100.0f)
	{
		m_PortalIncreaseF = 100.0f;
		m_PortalIncrease = 100;
		SceneManager::GetInstance()->LoadScene(SceneManager::Win);
		return;
	}
}

//EnemyNomalのポータル周りのコード.
void Portal::EnemyToPortal()
{
	if (auto enemy = m_pEnemy.lock())
	{
		// 💡 【追加】Enemy がキャプチャ State 中はゲージ増加を停止
		// if (enemy->IsCapturingState()) 
		// {
		//     return; // ゲージ増加処理をスキップ
		// }
	}

	float deltaTime = Timer::GetInstance().DeltaTime();

	m_PortalIncreaseF += deltaTime * 20.0f; // 💡 ゲージ速度を調整

	m_PortalIncrease = static_cast<int>(m_PortalIncreaseF);

	if (m_PortalIncreaseF >= 100.0f)
	{
		m_PortalIncreaseF = 100.0f;
		m_PortalIncrease = 100;
		SceneManager::GetInstance()->LoadScene(SceneManager::Lose);
		return;
	}
}