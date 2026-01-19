#pragma once
#include "Scene/SceneBase.h"

class SceneManager;
class UIObject;
class Sprite2D;
class DirectX11;

/*********************************************
*	エンディング画像クラス.
*	ToDo : Win画像の作成.
**/

class Ending
	: public SceneBase
{
public:
	enum class SelectMenu : byte
	{
		End,		//タイトル.
		Continue	//Firstシーンへ遷移.
	};

	enum class WinState : byte
	{
		Select,		//タイトルに戻るかコンティニューするか.
		FadeOut,	//コンティニューした際に再生.
		First,		//FadeOut後にFirstRoundへ遷移させる.
	};
public:
	Ending();
	~Ending() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:
	void UpdateSelect();
	void UpdateFadeOut();

private:
	SelectMenu	m_Select;
	WinState	m_State;

	//ずらす用のEndの位置
	D3DXVECTOR3 m_EndPos;
	//End画像の表示位置
	//ToDo : コンティニューと違和感のない配置にする.
	D3DXVECTOR3 m_EndSelectPos;
	//コンティニューの位置.
	D3DXVECTOR3 m_ContinuePos;

//ToDo : スマートポインタをまとめている.
#pragma region SmartPointer
	std::shared_ptr<Sprite2D> m_pSpriteBack;
	std::shared_ptr<UIObject> m_upBack;

	std::shared_ptr<Sprite2D> m_pSpriteVictory;
	std::shared_ptr<UIObject> m_upVictory;
#pragma endregion
};