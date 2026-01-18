#pragma once
#include "Scene/SceneBase.h"

class UIObject;
class Sprite2D;

/*********************************************
*	タイトル画像クラス.
**/

class Title
	: public SceneBase
{
public:
	//開始するか終わらせるかの選択.
	enum class SelectMenu : byte
	{
		Start,
		End
	};
public:
	Title();
	~Title() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	void Decide();

private:

	SelectMenu m_Select;

	std::shared_ptr<Sprite2D> m_pSpriteTitle;
	std::shared_ptr<UIObject> m_upTitle;

	std::shared_ptr<Sprite2D> m_pSpriteStart;
	std::shared_ptr<UIObject> m_upStart;

	std::shared_ptr<Sprite2D> m_pSpriteEnd;
	std::shared_ptr<UIObject> m_upEnd;
};