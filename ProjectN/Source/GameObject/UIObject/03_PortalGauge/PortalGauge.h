#pragma once

#include "..//UIObject.h"

class Sprite2D;
class DirectX11;

/*************************************************************
*	ポータルゲージUIクラス.
*	ToDo : このクラスでは、ポータルのUIの増加をするクラス.
**/

class PortalGauge
	: public UIObject
{
public:
	PortalGauge();
	~PortalGauge() override;

	void Update() override;
	void Draw() override;
	void Create() override;

	void SetPercent(float percent) { m_percent = percent; }

private:
	//ゲージのメンバ変数.
	std::shared_ptr<Sprite2D> m_spBaseSprite;  // 外枠
	std::shared_ptr<Sprite2D> m_spGaugeSprite; // ゲージ本体

	// UIオブジェクト
	std::shared_ptr<UIObject> m_upBase;
	std::shared_ptr<UIObject> m_upGauge;

	float m_maxBarScaleX; // 枠に収まる最大スケール
	float m_percent;      // 目標進捗率
	float m_currentScaleX; // 現在の表示スケール (アニメーション用)
	float m_barWidth;     // ゲージテクスチャの元幅
};