#include "Portal.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"
#include "System/02_Singleton/Timer/Timer.h"
#include "GameObject/SkinMeshObject/Character/Player/Player.h"
#include "GameObject/SkinMeshObject/Character/EnemyBase/00_Boss/Boss.h"

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

//DEBUG時にだけ表示されるImGuiの確認コード.
#ifdef _DEBUG

	//表示するブロック.
	ImGui::Begin(JAPANESE("Portal : 増加量/占有者")); // ウィンドウタイトルを分かりやすく変更

	const char* owner_text;
	switch (m_pPortalState) {
	case PortalPriority::Player:
		owner_text = "Player";
		//プレイヤーが触っているときの色(青).
		ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), JAPANESE("現在の占有者: %s"), owner_text);
		break;
	case PortalPriority::Enemy:
		owner_text = "Enemy";
		//敵が触っているときの色(赤).
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), JAPANESE("現在の占有者: %s"), owner_text);
		break;
	case PortalPriority::None:
	default:
		owner_text = "None";
		//誰も触っていないときの色(緑).
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), JAPANESE("現在の占有者: %s"), owner_text);
		break;
	}

	//スライダー(手動で触れるようにしている).
	//floatで表示させている.
	ImGui::SliderFloat(JAPANESE("Portalの取得パーセント(%)"), &m_PortalIncreaseF, 0.0f, 100.0f);
	//今進んでいるパーセンテージ.
	//スライダーの下にint型で表示している.
	ImGui::Text(JAPANESE("今のポータルのパーセント : %d"), m_PortalIncrease);

	//ポータルの取得パーセントが100%になったら文字が表示される.
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

	m_IsRoundFinished = false;
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

	// アニメーション起動.
	if (m_IsPlayerPriority && !m_IsEnemyPriority)
	{
		//Playerのみ範囲内: Playerに切り替え
		if (m_pPortalState != PortalPriority::Player)
		{
			if (auto player = m_pPlayer.lock())
			{
				//1秒間Stateを固定させる.
				player->SetCaptureState(1.0f); 
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
			//Playerと同じ感じのを作成する.
			m_FirstEnterPriority = PortalPriority::Enemy;
		}
		m_pPortalState = PortalPriority::Enemy;
	}
	else if (m_IsPlayerPriority && m_IsEnemyPriority)
	{
		//ポータルまでの競争時: 距離ではなく先に範囲に入っていたPlayer or EnemyNomalを優先にする.
		m_pPortalState = m_FirstEnterPriority;
	}
	else
	{
		//何も書かない.
	}
}
//Playerのポータル周りのコード(触った時とか・触った後の主導権).
void Portal::PlayerToPortal()
{
	if (m_IsRoundFinished)
	{
		//すでにラウンド終了している、スコアは増やさない.
		return; 
	}

	//Playerがポータルを触っているとき.
	if (auto player = m_pPlayer.lock())
	{
		if (player->IsCapturingState())
		{
			return; // アニメーション中はここで処理を終了し、ゲージ増加を停止
		}
	}

	float deltaTime = Timer::GetInstance().DeltaTime();
	m_PortalIncreaseF += deltaTime;
	m_PortalIncrease = static_cast<int>(m_PortalIncreaseF);

	if (m_PortalIncreaseF >= 100.0f)
	{
		m_PortalIncreaseF = 100.0f;
		m_PortalIncrease = 100;

		m_IsRoundFinished = true;

		SceneManager::GetInstance()->AddPlayerScore();

		SceneManager::GetInstance()->LoadScene(SceneManager::OP);

		return;
	}
}

//EnemyNomalのポータル周りのコード(触った時とか・触った後の主導権).
void Portal::EnemyToPortal()
{

	float deltaTime = Timer::GetInstance().DeltaTime();

	m_PortalIncreaseF += deltaTime;

	m_PortalIncrease = static_cast<int>(m_PortalIncreaseF);

	if (m_PortalIncreaseF >= 100.0f)
	{
		m_PortalIncreaseF = 100.0f;
		m_PortalIncrease = 100;
		SceneManager::GetInstance()->AddEnemyScore();

		SceneManager::GetInstance()->LoadScene(SceneManager::OP);
		return;
	}
}