#include "EnemyNomalManager.h"


void EnemyNomalManager::SetTargetPos(const D3DXVECTOR3& playerPos)
{
	// m_pEnemyNomal ベクトル内の全ての敵を巡回
	for (const auto& enemy : m_pEnemyNomal)
	{
		// unique_ptr が有効なポインタを保持しているか確認
		if (enemy)
		{
			// EnemyNomalはEnemyBaseを継承しており、
			// EnemyBase::SetTargetPos(const D3DXVECTOR3 Player_Pos) を持つ
			enemy->SetTargetPos(playerPos);
		}
	}
}

EnemyNomalManager::EnemyNomalManager()
{
}

EnemyNomalManager::~EnemyNomalManager()
{
}

EnemyNomalManager* EnemyNomalManager::GetInstace()
{
	static EnemyNomalManager s_Instance;
	return &s_Instance;
}

void EnemyNomalManager::Update()
{
	for (size_t i = 0; i < m_pEnemyNomal.size(); ++i)
	{
		if (m_pEnemyNomal[i] != nullptr)
		{
			m_pEnemyNomal[i]->Update();

			//敵が死亡していたら削除(今はないのでおいおい追加).
		}
	}
}

void EnemyNomalManager::Draw()
{
	for (size_t i = 0; i < m_pEnemyNomal.size(); ++i)
	{
		if (m_pEnemyNomal[i] != nullptr)
		{
			m_pEnemyNomal[i]->Draw();
		}
	}
}

void EnemyNomalManager::Create()
{
}

void EnemyNomalManager::Release()
{
}

void EnemyNomalManager::ReMoveEnemy(size_t index)
{
	if (index < m_pEnemyNomal.size())
	{
		m_pEnemyNomal.erase(m_pEnemyNomal.begin() + index);   //コンテナから要素を削除.
	}

}

EnemyNomal* EnemyNomalManager::GetEnemyNomal(size_t No)
{
	if (No < m_pEnemyNomal.size())
	{
		return m_pEnemyNomal[No].get();
	}
	return nullptr;
}

size_t EnemyNomalManager::GetEnemyNomalCount() const
{
	return m_pEnemyNomal.size();
}

void EnemyNomalManager::AddEnemy(std::unique_ptr<EnemyNomal> enemy, const D3DXVECTOR3& pos)
{
	enemy->SetEnemyPosition(pos); // ここでラッパー関数を使う
	m_pEnemyNomal.push_back(std::move(enemy));
}

std::vector<EnemyNomal*> EnemyNomalManager::GetEnemies()
{
	// 生のポインタを格納するための新しいベクターを宣言
	std::vector<EnemyNomal*> rawPointers;

	// 内部の unique_ptr のリストを走査
	for (const auto& enemy : m_pEnemyNomal)
	{
		// unique_ptr が有効な場合、生のポインタを取得して新しいベクターに追加
		if (enemy)
		{
			rawPointers.push_back(enemy.get());
		}
	}

	return rawPointers; // 値として返す}
}
