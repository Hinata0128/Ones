#pragma once
#include "Scene//SceneBase.h"	//基底クラス.

/**************************************************************************************
*	ファーストラウンド開始時に表示させる画像のクラス.
*	
*	作成したいイメージ: FirstRound->Redy Go!みたいな感じのものを作成したい.
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