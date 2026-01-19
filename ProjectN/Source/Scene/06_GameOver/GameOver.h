#pragma once
#include "Scene/SceneBase.h"

class SceneManager;
class UIObject;
class Sprite2D;
class DirectX11;

/*********************************************
*	ゲームオーバー画像クラス.
*	ToDo : Lose画面の作成.
**/

class GameOver
	: public SceneBase
{
public:
	enum class SelectMenu : byte
	{
		Continue,	//Firstシーンへ遷移.
		End			//Titleに遷移.
	};

	enum class LoseState : byte
	{
		Select,		//コンティニューかタイトルへ戻すか.
		FadeOut,	//Firstシーンに入る前に再生.
		First,		//FadeOut後にFirstRoundに遷移させる.
	};
public:
	GameOver();
	~GameOver() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:
	SelectMenu	m_Select;
	LoseState	m_State;

//ToDo : スマートポインタをまとめている.
#pragma region ImageList
	//背景画像.
	std::shared_ptr<Sprite2D> m_pSpriteBack;
	std::shared_ptr<UIObject> m_upBack;
	//敗北画像.
	std::shared_ptr<Sprite2D> m_pSpriteDefeat;
	std::shared_ptr<UIObject> m_upDefeat;
	//コンティニュー画像.
	std::shared_ptr<Sprite2D> m_pSpriteContinue;
	std::shared_ptr<UIObject> m_upContinue;
	//タイトルへ戻る用のEnd画像.
	std::shared_ptr<Sprite2D> m_pSpriteEnd;
	std::shared_ptr<UIObject> m_upEnd;
#pragma endregion

};