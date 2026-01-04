#pragma once

#include "..//UIObject.h"	//基底クラス.
#include <vector>

class Sprite2D;

/******************************************************
*	PointClass.
*	内容：陣地取得時に増えるポイントを管理するクラス.
*		:ImGuiを使用してカウントを追加させる.
**/

class Point
	: public UIObject
{
public:
	Point();
	~Point() override;

	void Update() override;
	void Draw() override;
	void Create() override;

	// ポイントの表示を切り替える関数 (indexは0か1)
	void SetPlayerPointActive(int index, bool active) { m_isPlayerActive[index] = active; }
	void SetBossPointActive(int index, bool active) { m_isBossActive[index] = active; }

private:
	// 画像リソース（2種類を使い回す）
	std::shared_ptr<Sprite2D> m_spBaseSprite;   // 未取得用
	std::shared_ptr<Sprite2D> m_spActiveSprite; // 取得済み用

	// プレイヤー用：[0]が1個目、[1]が2個目
	std::shared_ptr<UIObject> m_upPlayerBase[2];
	std::shared_ptr<UIObject> m_upPlayerActive[2];

	// ボス用：[0]が1個目、[1]が2個目
	std::shared_ptr<UIObject> m_upBossBase[2];
	std::shared_ptr<UIObject> m_upBossActive[2];

	// 各ポイントが有効かどうかのフラグ
	bool m_isPlayerActive[2];
	bool m_isBossActive[2];
};