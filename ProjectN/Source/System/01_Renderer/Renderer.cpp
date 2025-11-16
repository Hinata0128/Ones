#include "Renderer.h"
// 必要に応じて LIGHT, CAMERA 構造体の完全な定義を含むヘッダーをインクルードしてください

// ----------------------------------------------------
// シングルトンの実装
// ----------------------------------------------------
Renderer& Renderer::GetInstance()
{
	// C++11以降でスレッドセーフが保証される最も一般的なシングルトンパターン
	static Renderer s_instance;
	return s_instance;
}

// ----------------------------------------------------
// コンストラクタ / デストラクタの実装
// ----------------------------------------------------
Renderer::Renderer()
{
	// メンバー変数の初期化処理を記述します。
	// 例: D3DXMatrixIdentity(&m_View); など、安全な初期値を設定します。
}

Renderer::~Renderer()
{
	// クリーンアップ処理
}

// ----------------------------------------------------
// メソッドの実装
// ----------------------------------------------------

void Renderer::SetDrawParameters(const D3DXMATRIX& view, const D3DXMATRIX& proj, const LIGHT& light, const CAMERA& camera)
{
	m_View = view;
	m_Proj = proj;
	m_Light = light;
	m_Camera = camera;
}

// ゲッターの実装 (すべて const メソッド)
const D3DXMATRIX& Renderer::GetView() const
{
	return m_View;
}

const D3DXMATRIX& Renderer::GetProj() const
{
	return m_Proj;
}

const LIGHT& Renderer::GetLight() const
{
	return m_Light;
}

const CAMERA& Renderer::GetCamera() const
{
	return m_Camera;
}