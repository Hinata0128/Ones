#include "Main.h"
#include "DirectX\\DirectX9.h"
#include "DirectX\\DirectX11.h"
#include "SceneManager/SceneManager.h"
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "Effect/Effect.h"
#include "Sound/SoundManager.h"

#include "System/00_Manager/03_ImGuiManager/ImGuiManager.h"
// ImGuiのメッセージハンドラのための外部宣言。通常はImguiManager.hに含まれます。
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//ウィンドウを画面中央で起動を有効にする.
//#define ENABLE_WINDOWS_CENTERING

//=================================================
//	定数.
//=================================================
const TCHAR WND_TITLE[] = _T("Ones");
const TCHAR APP_NAME[] = _T("Ones");
// 外部定義のFPS定数 (Loop関数で使用)
// const int FPS = 60; 


//=================================================
//	コンストラクタ.
//=================================================
Main::Main()
//初期化リスト.
	: m_hWnd(nullptr)
	// m_SomeFloatValue, m_bFeatureEnabled は Main.h の初期化リストで設定されていることを想定
{
	AllocConsole(); // コマンドプロンプト表示
}


//=================================================
//	デストラクタ.
//=================================================
Main::~Main()
{
	DeleteObject(m_hWnd);
}


//=================================================
//	構築処理.
//=================================================
HRESULT Main::Create()
{
	auto pDx9 = DirectX9::GetInstance();
	auto pDx11 = DirectX11::GetInstance();

	if (FAILED(pDx9->Create(m_hWnd)))
		return E_FAIL;
	if (FAILED(pDx11->Create(m_hWnd)))
		return E_FAIL;

	StaticMeshManager::GetInstance()->Create();
	SkinMeshManager::GetInstance()->Create();

	SceneManager::GetInstance()->SetDx9(pDx9);
	SceneManager::GetInstance()->SetDx11(pDx11);

	Effect::GetInstance()->Create(pDx11->GetDevice(), pDx11->GetContext());

	// ImGui 初期化
	if (FAILED(ImGuiManager::GetInstance()->Init(m_hWnd)))
		return E_FAIL;

	return S_OK;
}

//=================================================
//	更新処理.
//=================================================
void Main::Update()
{
	ImGuiManager::GetInstance()->NewFrameSetting();

	SceneManager::GetInstance()->Update();

	ImGui::Begin("My Debug Window");
	ImGui::Text("Hello, ImGui!");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::Button("Test Button"))
	{
		OutputDebugString(_T("Test Button Clicked!\n"));
	}
	// ★ ImGuiの初期位置設定（移動できない場合はこれをコメントアウトしてください）
	// ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once); 

	ImGui::SliderFloat("Float Value", &m_SomeFloatValue, 0.0f, 100.0f);
	ImGui::Checkbox("Enable Feature", &m_bFeatureEnabled);

	ImGui::End();
}

//=================================================
//	描画処理.
//=================================================
void Main::Draw()
{
	auto pDx11 = DirectX11::GetInstance();

	//バックバッファをクリア
	pDx11->ClearBackBuffer();

	SceneManager::GetInstance()->Draw();

	ImGuiManager::GetInstance()->Render();

	// 画面に表示
	pDx11->Present();
}


//=================================================
//	データロード処理.
//=================================================
HRESULT Main::LoadData()
{
	if (SoundManager::GetInstance()->Load(m_hWnd) == false) {
		return E_FAIL;
	}

	if (FAILED(Effect::GetInstance()->LoadData()))
	{
		return E_FAIL;
	}

	SceneManager::GetInstance()->Create(m_hWnd);

	return S_OK;
}


//=================================================
//	解放処理.
//=================================================
void Main::Release()
{
	DirectX11::GetInstance()->Release();
	DirectX9::GetInstance()->Release();
}


//=================================================
//	メッセージループ.
//=================================================
void Main::Loop()
{
	if (FAILED(LoadData())) {
		return;
	}

	//------------------------------------------------
	//	フレームレート調整準備.
	//------------------------------------------------
	float Rate = 0.0f;
	DWORD sync_old = timeGetTime();
	DWORD sync_now;

	timeBeginPeriod(1);
	// FPSは外部で定義されていると仮定
	Rate = 1000.0f / static_cast<float>(FPS);

	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		sync_now = timeGetTime();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (sync_now - sync_old >= Rate)
		{
			sync_old = sync_now;

			// 更新と描画を分離して呼び出し
			Update();
			Draw();
		}
	}

	timeEndPeriod(1); // timeBeginPeriodと対で呼ぶ
	Release();
}

//=================================================
//	ウィンドウ初期化関数.
//=================================================
HRESULT Main::InitWindow(
	HINSTANCE hInstance,
	INT x, INT y,
	INT width, INT height)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MsgProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = APP_NAME;
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		_ASSERT_EXPR(false, _T("ウィンドウクラスの登録に失敗"));
		return E_FAIL;
	}

	// ... (ウィンドウ表示位置の調整ロジックは簡略化) ...

	RECT	rect = { 0, 0, width, height };
	DWORD	dwStyle = WS_OVERLAPPEDWINDOW;

	if (AdjustWindowRect(&rect, dwStyle, FALSE) == 0)
	{
		MessageBox(nullptr, _T("ウィンドウ領域の調整に失敗"), _T("エラーメッセージ"), MB_OK);
		return E_FAIL;
	}

	INT winWidth = rect.right - rect.left;
	INT winHeight = rect.bottom - rect.top;

	m_hWnd = CreateWindow(
		APP_NAME, WND_TITLE, dwStyle,
		0, 0, // 簡略化して左上に表示
		winWidth, winHeight,
		nullptr, nullptr, hInstance, nullptr);

	if (!m_hWnd) {
		_ASSERT_EXPR(false, _T("ウィンドウ作成失敗"));
		return E_FAIL;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}


//=================================================
//	ウィンドウ関数（メッセージ毎の処理）.
//=================================================
LRESULT CALLBACK Main::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	// ★ 修正: ImGuiのメッセージハンドラを最初に呼び出し、UIがメッセージを消費したら処理を中断します
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true; // ImGuiがイベントを処理した
	}

	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (static_cast<char>(wParam)) {
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}