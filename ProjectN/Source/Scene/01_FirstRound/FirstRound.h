#pragma once
#include "Scene//SceneBase.h"	//基底クラス.

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

};