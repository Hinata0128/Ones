#pragma once

#include "GameObject/GameObject.h"
#include "Sprite2D/Sprite2D.h"

/************************************************************
*	UIオブジェクトクラス.
**/
class UIObject : public GameObject
{
public:
	UIObject();
	virtual ~UIObject() override;

	// Update と Draw（引数なし）
	virtual void Update() override;
	virtual void Draw() override final; // ← ここでfinal指定OK！

	// スプライトを接続する
	void AttachSprite(Sprite2D& pSprite) { m_pSprite = &pSprite; }
	void DetachSprite() { m_pSprite = nullptr; }

	// パターン番号を設定
	void SetPatternNo(SHORT x, SHORT y)
	{
		m_PatternNo.x = x;
		m_PatternNo.y = y;
	}

protected:
	Sprite2D* m_pSprite;
	POINTS		m_PatternNo;	// パターン番号(マス目)
};
