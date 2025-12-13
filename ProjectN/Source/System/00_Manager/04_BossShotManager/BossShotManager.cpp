#include "BossShotManager.h"

BossShotManager* BossShotManager::m_pInstance = nullptr;

BossShotManager::BossShotManager()
{
}

BossShotManager::~BossShotManager()
{
}

BossShotManager* BossShotManager::GetInstance()
{
	static BossShotManager s_Instance;	//唯一のインスタンス生成.
	return &s_Instance;
}

void BossShotManager::Update()
{
	for (auto it = m_pBossShot.begin(); it != m_pBossShot.end();)
	{
		(*it)->Update();	//弾の更新処理.

		if (!(*it)->IsActive())	//非アクティブな弾は削除.
		{
			it = m_pBossShot.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BossShotManager::Draw()
{
	for (std::unique_ptr<BossShot>& ENomalShot : m_pBossShot)
	{
		ENomalShot->Draw();	//弾の描画処理.
		ENomalShot->GetBoundingSphere().Draw();

	}
}

void BossShotManager::Init()
{
	m_pBossShot.clear();
}

void BossShotManager::AddEnemyNomalShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirecton)
{
	constexpr float Speed = 0.05f;	//弾の速度設定.

	auto ENomalShot = std::make_unique<BossShot>();	//弾の生成.

	ENomalShot->Reload(Pos, InitDirecton, Speed);	//弾の初期化.

	ENomalShot->SetDisplay(true);	//表示有効化.

	m_pBossShot.push_back(std::move(ENomalShot));	//リストに追加.
}

void BossShotManager::ReMoveEnemyNomalShot(size_t index)
{
	if (index < m_pBossShot.size())
	{
		m_pBossShot.erase(m_pBossShot.begin() + index);	//指定弾削除.
	}
}

const std::vector<std::unique_ptr<BossShot>>& BossShotManager::GetEnemyNomalShot() const
{
	return m_pBossShot;	//弾リスト返却.
}

BossShot* BossShotManager::GetEnemyNomalShot(size_t No)
{
	if (No < m_pBossShot.size())
	{
		return m_pBossShot[No].get();	//指定弾返却.
	}
	return nullptr;
}

size_t BossShotManager::GetEnemyNomalShotCount() const
{
	return m_pBossShot.size();	//弾数返却.
}

std::vector<BossShot*> BossShotManager::GetShots()
{
	// 生のポインタを格納するための新しいベクターを宣言
	std::vector<BossShot*> rawPointers;

	// 内部の unique_ptr のリストを走査
	for (const auto& shot : m_pBossShot)
	{
		// unique_ptr が有効な場合、生のポインタを取得して新しいベクターに追加
		if (shot)
		{
			rawPointers.push_back(shot.get());
		}
	}
	return rawPointers; // 値として返す
}