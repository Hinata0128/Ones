#pragma once
#include "Scene/SceneBase.h"

class SceneManager;
class UIObject;
class Sprite2D;
class DirectX11;

/*********************************************
*	エンディング画像クラス.
*	ToDo : Win画像の作成.
**/

class Ending
	: public SceneBase
{
public:
	Ending();
	~Ending() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:
	std::shared_ptr<Sprite2D> m_pSpriteBack;
	std::shared_ptr<UIObject> m_upBack;

	std::shared_ptr<Sprite2D> m_pSpriteVictory;
	std::shared_ptr<UIObject> m_upVictory;
};