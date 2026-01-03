#pragma once

#include "..//UIObject.h"
#include <memory>
#include <vector>

class Sprite2D;

/***************************************************************
*	HPBarの表示クラス
*	4枚の画像を重ねてHPバーを構成します
**/

class HPBar : public UIObject
{
public:
	HPBar();
	~HPBar() override;

	void Update() override;
	void Draw() override;
	void Create() override;

private:
	// 各パーツ用のSprite
	std::shared_ptr<Sprite2D> m_spBaseSprite;   // HPBase (枠)
	std::shared_ptr<Sprite2D> m_spBackSprite;   // GaugeBack (背景)
	std::shared_ptr<Sprite2D> m_spDamageSprite; // HPDamage (赤)
	std::shared_ptr<Sprite2D> m_spGaugeSprite;  // HPGauge (緑)

	// 各パーツ用のUIオブジェクト
	std::shared_ptr<UIObject> m_upBase;
	std::shared_ptr<UIObject> m_upBack;
	std::shared_ptr<UIObject> m_upDamage;
	std::shared_ptr<UIObject> m_upGauge;
};