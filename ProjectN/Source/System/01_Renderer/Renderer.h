#pragma once

#include <iostream>
#include <D3DX9.h>
// D3DXMATRIX, D3DXVECTOR3 の定義を含む

// LIGHT, CAMERA 構造体の定義は外部ファイル（例：ゲーム共通ヘッダー）にあると仮定します。
// 例:
// struct LIGHT { ... };
// struct CAMERA { ... };


/************************************************************************************
*	レンダラークラス.
**/
class Renderer
{
public:
	// シングルトンインスタンスを取得する静的メソッド
	static Renderer& GetInstance();

	// 仮想デストラクタ (シングルトンでも継承される可能性があるため推奨)
	virtual ~Renderer();

	// --- 外部から描画パラメータを設定するメソッド ---
	void SetDrawParameters(const D3DXMATRIX& view, const D3DXMATRIX& proj, const LIGHT& light, const CAMERA& camera);

	// --- 各パラメータを取得するためのゲッター ---
	const D3DXMATRIX& GetView() const;
	const D3DXMATRIX& GetProj() const;
	const LIGHT& GetLight() const;
	const CAMERA& GetCamera() const;

private:
	// コンストラクタを private にして外部からの直接生成を禁止
	Renderer();

	// コピー、ムーブを禁止
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

private:
	// メンバー変数の定義
	D3DXMATRIX	m_View;
	D3DXMATRIX	m_Proj;

	CAMERA		m_Camera;
	LIGHT		m_Light;
};