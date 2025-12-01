#pragma once
#include "Scene//SceneBase.h"	//基底クラス.

#include "GameObject/UIObject/UIObject.h"

#include "Sprite2D/Sprite2D.h"

/**************************************************************************************
*	ファーストラウンド開始時に表示させる画像のクラス.
*	
*	作成したいイージ: 一枚の画像を表示して時間経過で実装できるようにする.
* 
**/

class FirstRound
	: public SceneBase
{
public:
	FirstRound();
	~FirstRound() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;
private:
	//FirstRoundの2Dスプライト表示.
	Sprite2D* m_pFirstRoundImg;
	std::shared_ptr<UIObject> m_pFirstRound;
};