#include "PShotManager.h"
#include "GameObject/SkinMeshObject/Character/Player/Player.h"

PShotManager::PShotManager()
{
}

PShotManager::~PShotManager()
{
}

PShotManager* PShotManager::GetInstance()
{
	static PShotManager s_Instance;	//唯一のインスタンス.
	return &s_Instance;
}

void PShotManager::Update()
{
	for (auto it = m_PlayerShot.begin(); it != m_PlayerShot.end();)
	{
		// std::unique_ptr<PShot> のポインタを通じて PShot::Update() を呼び出す。
		// m_pInstanceの矛盾解消により、コンテナの破壊を防ぐ。
		(*it)->Update();	//弾の更新処理.

		if (!(*it)->IsActive())	//無効になった弾は削除.
		{
			it = m_PlayerShot.erase(it); // unique_ptrが自動でdeleteを呼び出すため安全
		}
		else
		{
			++it;
		}
	}
}

void PShotManager::Draw()
{
	for (auto& shot : m_PlayerShot)
	{
		shot->Draw();	//弾の描画処理.
	}
}

void PShotManager::Init()
{
	m_PlayerShot.clear();
}

void PShotManager::AddPlayerShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirection)
{
	constexpr float Speed = 0.05f;	//弾速.

	auto PlayerShot = std::make_unique<PShot>();	//新規弾生成.
	PlayerShot->Reload(Pos, InitDirection, Speed);	//位置・方向・速度を設定.
	PlayerShot->SetDisplay(true);	//描画ON.

	m_PlayerShot.push_back(std::move(PlayerShot));	//リストに追加.
}

void PShotManager::ReMovePlayerShot(size_t index)
{
	if (index < m_PlayerShot.size())
	{
		m_PlayerShot.erase(m_PlayerShot.begin() + index);	//指定弾を削除.
	}
}

void PShotManager::ClearPlayerShot()
{
	m_PlayerShot.clear();
}

const std::vector<std::unique_ptr<PShot>>& PShotManager::GetPlayerShot()const
{
	return m_PlayerShot;	//弾リストを返す.
}

PShot* PShotManager::GetPlayerShot(size_t No)
{
	if (No < m_PlayerShot.size())
	{
		return m_PlayerShot[No].get();	//指定弾を取得.
	}
	return nullptr;	//範囲外.
}

size_t PShotManager::GetPlayerShotCount()const
{
	return m_PlayerShot.size();	//弾の総数を返す.
}

std::vector<PShot*> PShotManager::GetShots()
{
	std::vector<PShot*> rawPointers;
	for (const auto& shot : m_PlayerShot)
	{
		if (shot)
		{
			rawPointers.push_back(shot.get());
		}
	}
	return rawPointers; // 値として返す
}