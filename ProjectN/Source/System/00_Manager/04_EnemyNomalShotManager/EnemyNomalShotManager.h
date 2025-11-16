#pragma once
#include "GameObject/StaticMeshObject/ShotBase/EnemyNomalShot/EnemyNomalShot.h"

/*****************************************************************
*	敵の通常攻撃マネージャークラス.
**/
class EnemyNomalShotManager
{
public:
	~EnemyNomalShotManager();

	static EnemyNomalShotManager* GetInstance();

	void Update();	//弾の更新処理.
	void Draw();	//弾の描画処理.

	void AddEnemyNomalShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirecton);	//敵の弾を作成して管理リストに追加.

	void ReMoveEnemyNomalShot(size_t index);	//敵の弾を削除.

	const std::vector<std::unique_ptr<EnemyNomalShot>>& GetEnemyNomalShot() const;	//敵の弾リスト取得.

	EnemyNomalShot* GetEnemyNomalShot(size_t No);	//敵の弾取得.

	size_t GetEnemyNomalShotCount() const;	//敵の弾数取得.

	std::vector<EnemyNomalShot*> GetShots();
private:
	EnemyNomalShotManager();

	EnemyNomalShotManager(const EnemyNomalShotManager& rhs) = delete;	//コピー禁止.
	EnemyNomalShotManager& operator = (const EnemyNomalShotManager& rhs) = delete;	//代入禁止.
private:
	static EnemyNomalShotManager* m_pInstance;	//シングルトンインスタンス.

	std::vector<std::unique_ptr<EnemyNomalShot>> m_EnemyNomalShot;	//敵弾の管理配列.
};