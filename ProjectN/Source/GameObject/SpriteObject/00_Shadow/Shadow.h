#pragma once

#include "..//SpriteObject.h"

class DirectX11;

/***********************************************************
*	影クラス.
*	ToDo : プレイヤー・ボス・弾の影を実装.
**/

class Shadow
	: public SpriteObject
{
public:
	Shadow();
	~Shadow() override;

	void Update() override;
	void Draw() override;
	//画像の命名.
	void Create() override;

private:

};