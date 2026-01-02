#pragma once
#include "Scene/SceneBase.h"
#include "GameObject/SpriteObject/SpriteObject.h"
#include "SceneManager/SceneManager.h"

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
};