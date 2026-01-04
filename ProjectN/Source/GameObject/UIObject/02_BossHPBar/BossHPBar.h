#pragma once

#include "..//UIObject.h"
#include <vector>
#include "GameObject/SkinMeshObject/Character/EnemyBase/00_Boss/Boss.h"

class Sprite2D;

/***********************************************************
*	BossHPBar‚Ì•\¦ƒNƒ‰ƒX.
**/

class BossHPBar
	: public UIObject
{
public:
	BossHPBar();
	~BossHPBar() override;

	void Update() override;
	void Draw() override;
	void Create() override;

	//ƒ{ƒX‚Ì‘Ì—ÍŒ¸­.
	void SetTragetBoss(Boss* boss) { m_pTargetBoss = boss; }

private:
	Boss* m_pTargetBoss;
	float m_MaxBarScaleX;

	//Sprite2D‚Ìƒƒ“ƒo•Ï”.
	std::shared_ptr<Sprite2D> m_spBaseSprite;	//˜g.
	std::shared_ptr<Sprite2D> m_spBackSprite;	//ŠDF‚Ì”wŒi.
	std::shared_ptr<Sprite2D> m_spDamageSprite;	//ÔF‚Ì”wŒi.
};