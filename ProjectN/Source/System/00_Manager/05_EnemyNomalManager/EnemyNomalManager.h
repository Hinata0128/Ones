#pragma once
#include "GameObject//SkinMeshObject//Character//EnemyBase//EnemyNomal//EnemyNomal.h"
#include "System//00_Manager//ManagerBase.h"

/*********************************************************************************
*	ノーマルの敵の数を増やすのでマネージャークラスを作成.
*	シングルトンで作成します.
**/

class EnemyNomalManager
	: public ManagerBase
{
public:
	~EnemyNomalManager();

	static EnemyNomalManager* GetInstace();

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	//敵の破棄.
	void ReMoveEnemy(size_t index);

	EnemyNomal* GetEnemyNomal(size_t No);

	//ノーマルの敵の数を取得する.
	size_t GetEnemyNomalCount() const;

	// ここでクラス名は不要
	void AddEnemy(std::unique_ptr<EnemyNomal> enemy, const D3DXVECTOR3& pos);

	std::vector<EnemyNomal*> GetEnemies();

	// プレイヤーの位置を全敵に設定する関数を追記
	void SetTargetPos(const D3DXVECTOR3& playerPos);
private:
	EnemyNomalManager();

	EnemyNomalManager(const EnemyNomalManager& rhs) = delete;
	EnemyNomalManager& operator = (const EnemyNomalManager& rhs) = delete;
private:
	std::vector<std::unique_ptr<EnemyNomal>> m_pEnemyNomal;
};