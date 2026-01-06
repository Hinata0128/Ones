#pragma once

#include "..//UIObject.h"

class Sprite2D;
class DirectX11;
class Timer;

/*********************************************************************
*	制限時間クラス.
*	ToDo : 全ラウンド共通のもの.
**/

class LimitTime
	: public UIObject
{
public:
	LimitTime();
	~LimitTime() override;

	void Create() override;
	void Update() override;
	void Draw() override;
private:

	//制限時間に必要なメンバ変数.
	std::shared_ptr<Sprite2D> m_pClockBackSprite;
	std::shared_ptr<Sprite2D> m_pClockFrameSprite;
	std::shared_ptr<Sprite2D> m_pClockFrontSprite;

	std::shared_ptr<UIObject> m_upClockBack;
	std::shared_ptr<UIObject> m_upClockFrame;
	std::shared_ptr<UIObject> m_upClockFront;
};