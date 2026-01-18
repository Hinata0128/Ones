#pragma once
#include "Scene/SceneBase.h"

class UIObject;
class Sprite2D;

/*********************************************
*	タイトル画像クラス.
**/

class Title
	: public SceneBase
{
public:
	Title();
	~Title() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:
	std::shared_ptr<Sprite2D> m_pSpriteTitle;
	std::shared_ptr<UIObject> m_upTitle;
};