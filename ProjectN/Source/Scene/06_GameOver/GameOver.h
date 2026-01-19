#pragma once
#include "Scene/SceneBase.h"

class SceneManager;
class UIObject;
class Sprite2D;
class DirectX11;

/*********************************************
*	ゲームオーバー画像クラス.
*	ToDo : Lose画面の作成.
**/

class GameOver
	: public SceneBase
{
public:
	GameOver();
	~GameOver() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:
	std::shared_ptr<Sprite2D> m_pSpriteBack;
	std::shared_ptr<UIObject> m_upBack;
	
	std::shared_ptr<Sprite2D> m_pSpriteDefeat;
	std::shared_ptr<UIObject> m_upDefeat;
};