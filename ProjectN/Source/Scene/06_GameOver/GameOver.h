#pragma once
#include "Scene/SceneBase.h"
#include "GameObject/SpriteObject/SpriteObject.h"
#include "SceneManager/SceneManager.h"

#include "Sprite2D/Sprite2D.h"
#include "GameObject/UIObject/UIObject.h"

/*********************************************
*	ゲームオーバー画像クラス.
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
	//HPバーの2Dスプライト.
	std::shared_ptr<Sprite2D> m_pHpBarSprite;
	//UIObject* m_pHpBar;
	std::shared_ptr<UIObject> m_pHpBar;
};