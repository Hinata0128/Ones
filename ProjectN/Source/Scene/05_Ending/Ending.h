#pragma once
#include "Scene/SceneBase.h"
#include "GameObject/01_SpriteObject/SpriteObject.h"
#include "SceneManager/SceneManager.h"

#include "GameObject/02_StaticMeshObject/01_Portal/Portal.h"

/*********************************************
*	エンディング画像クラス.
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
	std::unique_ptr<Portal> m_pPortal;
};