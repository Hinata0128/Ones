#pragma once

#include "..//UIObject.h"

class Sprite2D;

/***************************************************************
*	HPBarの表示クラス(Test).
*	UIObjectを親として継承させる方法で作成していく.
**/

class HPBar
	: public UIObject
{
public:
	HPBar();
	~HPBar() override;

	//動作関数.
	void Update() override;
	//表示関数.
	void Draw() override;

	void Create() override;
private:
	//HPバーの2Dスプライト.
	std::shared_ptr<Sprite2D> m_pHpBarSprite;
	//UIObject* m_pHpBar;
	std::shared_ptr<UIObject> m_pHpBar;
};