#pragma once
#include <Windows.h>

//クラスの前方宣言.
class DirectX9;
class DirectX11;
class Game;
class SceneManager;
class StaticMeshManager;
class SkinMeshManager;
class ImGuiManager;

/**************************************************
*	メインクラス.
**/
class Main
{
public:
	Main();	//コンストラクタ.
	~Main();	//デストラクタ.

	void Update();		//更新処理.
	void Draw();		//描画関数.
	HRESULT Create();	//構築処理.
	HRESULT LoadData();	//データロード処理.
	void Release();		//解放処理.
	void Loop();		//メインループ.

	//ウィンドウ初期化関数.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y,
		INT width, INT height );

private:
	//ウィンドウ関数（メッセージ毎の処理）.
	static LRESULT CALLBACK MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam );

private:
	HWND			m_hWnd;	//ウィンドウハンドル.

	float m_SomeFloatValue = 0.0f;
	bool m_bFeatureEnabled = false;

};