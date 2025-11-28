#include "EnemyNomalShotManager.h"

EnemyNomalShotManager* EnemyNomalShotManager::m_pInstance = nullptr;

EnemyNomalShotManager::EnemyNomalShotManager()
{
}

EnemyNomalShotManager::~EnemyNomalShotManager()
{
}

EnemyNomalShotManager* EnemyNomalShotManager::GetInstance()
{
	static EnemyNomalShotManager s_Instance;	//唯一のインスタンス生成.
	return &s_Instance;
}

void EnemyNomalShotManager::Update()
{
	for (auto it = m_EnemyNomalShot.begin(); it != m_EnemyNomalShot.end();)
	{
		(*it)->Update();	//弾の更新処理.

		if (!(*it)->IsActive())	//非アクティブな弾は削除.
		{
			it = m_EnemyNomalShot.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void EnemyNomalShotManager::Draw()
{
	for (std::unique_ptr<EnemyNomalShot>& ENomalShot : m_EnemyNomalShot)
	{
		ENomalShot->Draw();	//弾の描画処理.
		ENomalShot->GetBoundingSphere().Draw();

	}
}

void EnemyNomalShotManager::Init()
{
	m_EnemyNomalShot.clear();
}

void EnemyNomalShotManager::AddEnemyNomalShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirecton)
{
	constexpr float Speed = 0.05f;	//弾の速度設定.

	auto ENomalShot = std::make_unique<EnemyNomalShot>();	//弾の生成.

	ENomalShot->Reload(Pos, InitDirecton, Speed);	//弾の初期化.

	ENomalShot->SetDisplay(true);	//表示有効化.

	m_EnemyNomalShot.push_back(std::move(ENomalShot));	//リストに追加.
}

void EnemyNomalShotManager::ReMoveEnemyNomalShot(size_t index)
{
	if (index < m_EnemyNomalShot.size())
	{
		m_EnemyNomalShot.erase(m_EnemyNomalShot.begin() + index);	//指定弾削除.
	}
}

const std::vector<std::unique_ptr<EnemyNomalShot>>& EnemyNomalShotManager::GetEnemyNomalShot() const
{
	return m_EnemyNomalShot;	//弾リスト返却.
}

EnemyNomalShot* EnemyNomalShotManager::GetEnemyNomalShot(size_t No)
{
	if (No < m_EnemyNomalShot.size())
	{
		return m_EnemyNomalShot[No].get();	//指定弾返却.
	}
	return nullptr;
}

size_t EnemyNomalShotManager::GetEnemyNomalShotCount() const
{
	return m_EnemyNomalShot.size();	//弾数返却.
}

std::vector<EnemyNomalShot*> EnemyNomalShotManager::GetShots()
{
	// 生のポインタを格納するための新しいベクターを宣言
	std::vector<EnemyNomalShot*> rawPointers;

	// 内部の unique_ptr のリストを走査
	for (const auto& shot : m_EnemyNomalShot)
	{
		// unique_ptr が有効な場合、生のポインタを取得して新しいベクターに追加
		if (shot)
		{
			rawPointers.push_back(shot.get());
		}
	}
	return rawPointers; // 値として返す
}