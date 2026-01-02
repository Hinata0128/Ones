#pragma once
#include "GameObject/StaticMeshObject/02_ShotBase/PShot/PShot.h"
#include <vector>
#include <memory> // std::unique_ptr のため

class Player;

/**********************************************
*プレイヤー弾マネージャークラス.
*弾の生成・更新・描画・破棄を管理する.
**********************************************/
class PShotManager
{
public:
	~PShotManager();

	static PShotManager* GetInstance();	//インスタンス取得.

	void Update();	//弾の更新処理.
	void Draw();	//弾の描画処理.
	void Init();

	void AddPlayerShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirection);	//プレイヤーの弾を追加.
	void ReMovePlayerShot(size_t index);	//指定インデックスの弾を削除.

	//Playerが死んだときにGameMain内にPlayerの弾が残るのを一斉に削除する.
	void ClearPlayerShot();

	const std::vector<std::unique_ptr<PShot>>& GetPlayerShot()const;	//弾リストを取得.
	PShot* GetPlayerShot(size_t No);	//指定番号の弾を取得.
	size_t GetPlayerShotCount()const;	//弾の総数を取得.

	std::vector<PShot*> GetShots();
private:
	PShotManager();
	PShotManager(const PShotManager& rhs) = delete;
	PShotManager& operator=(const PShotManager& rhs) = delete;

private:
	std::vector<std::unique_ptr<PShot>> m_PlayerShot;	//弾リスト.
};