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
	std::shared_ptr<Sprite2D> m_spBase;
	std::shared_ptr<Sprite2D> m_spGauge;

	//ゲージのメンバ変数.
	std::shared_ptr<UIObject> m_upPortalGaugeBaseUI;
	std::shared_ptr<UIObject> m_upPortalGaugeUI;

	float m_maxBarScaleX;
	float m_percent;
	float m_barWidth;
};