#include "StaticMeshObject.h"
// Renderer クラスの実装に必要なヘッダー
// #include "Renderer.h" // ヘッダーでインクルード済みだが、念のため記載

StaticMeshObject::StaticMeshObject()
	: m_pMesh(nullptr)
{
	// m_Position, m_Rotation, m_Scale など、GameObject のメンバ変数の初期化が必要です
	// (GameObject のコンストラクタで初期化されていることを前提とします)
}

StaticMeshObject::~StaticMeshObject()
{
	// 接続されたメッシュ（リソース自体）は、このオブジェクトが所有していない（生ポインターのため）
	// そのため、ここでは DetachMesh() で nullptr に設定するのみです。
	DetachMesh();
}

void StaticMeshObject::Update()
{
	if (m_pMesh == nullptr) {
		return;
	}
	// ここに位置の更新や衝突判定などのロジックが入ります。
}

void StaticMeshObject::Draw()
{
	if (!m_pMesh) return;

	// Rendererから描画パラメータ取得
	auto& renderer = Renderer::GetInstance();

	// 座標・回転・スケールを反映
	// m_Position などのメンバー変数が GameObject クラスで定義されていることを前提とします。
	m_pMesh->SetPosition(m_Position);
	m_pMesh->SetRotation(m_Rotation);
	m_pMesh->SetScale(m_Scale);

	// Render呼び出し
	// Rendererのゲッターと StaticMesh::Render の引数が const 参照 (const T&) で
	// 一致しているため、型エラーは解消されます。
	m_pMesh->Render(
		renderer.GetView(),
		renderer.GetProj(),
		renderer.GetLight(),
		renderer.GetCamera().vPosition // カメラ位置 (D3DXVECTOR3型を想定)
	);
}