#pragma once
#include "GameObject/StaticMeshObject/02_ShotBase/BossShot/BossShot.h"

/*****************************************************************
*	敵の通常攻撃マネージャークラス.
**/
class BossShotManager final
{
public:
	~BossShotManager();

	static BossShotManager* GetInstance();

	void Update();	//弾の更新処理.
	void Draw();	//弾の描画処理.

	void Init();

	void AddEnemyNomalShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirecton);	//敵の弾を作成して管理リストに追加.

	void ReMoveEnemyNomalShot(size_t index);	//敵の弾を削除.

	const std::vector<std::unique_ptr<BossShot>>& GetEnemyNomalShot() const;	//敵の弾リスト取得.

	BossShot* GetEnemyNomalShot(size_t No);	//敵の弾取得.

	size_t GetEnemyNomalShotCount() const;	//敵の弾数取得.

	std::vector<BossShot*> GetShots();
private:
	BossShotManager();

	BossShotManager(const BossShotManager& rhs) = delete;	//コピー禁止.
	BossShotManager& operator = (const BossShotManager& rhs) = delete;	//代入禁止.
private:
	static BossShotManager* m_pInstance;	//シングルトンインスタンス.

	std::vector<std::unique_ptr<BossShot>> m_pBossShot;	//敵弾の管理配列.
};