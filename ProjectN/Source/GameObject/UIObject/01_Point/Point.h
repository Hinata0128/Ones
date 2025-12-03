#pragma once

#include "..//UIObject.h"	//基底クラス.

class Sprite2D;

/******************************************************
*	PointClass.
*	内容：陣地取得時に増えるポイントを管理するクラス.
*		:ImGuiを使用してカウントを追加させる.
**/

class Point
	: public UIObject
{
public:
	Point();
	~Point() override;

	void Update() override;
	void Draw() override;

	void Create() override;

private:

};