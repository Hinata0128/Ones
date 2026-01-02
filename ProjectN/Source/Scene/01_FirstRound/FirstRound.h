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

class FirstRound final
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
	//Sprite2D* m_pFirstRoundImg;
	//std::shared_ptr<UIObject> m_pFirstRound;

	std::shared_ptr<Sprite2D> m_pSpriteTitle;
	std::shared_ptr<UIObject> m_pTitleObj;

	//時間経過でシーンを遷移する.
	//タイマーのメンバ変数.
	float m_Timer;

};