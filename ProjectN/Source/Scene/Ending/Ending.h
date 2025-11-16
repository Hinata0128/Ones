#pragma once
#include "Scene/CSceneBase.h"
#include "GameObject/SpriteObject/SpriteObject.h"
#include "SceneManager/SceneManager.h"

/*********************************************
*	エンディング画像クラス.
**/

class Ending
	: public CSceneBase
{
public:
	Ending();
	~Ending() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:

};