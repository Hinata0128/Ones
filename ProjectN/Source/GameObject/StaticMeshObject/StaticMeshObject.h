#pragma once

#include "GameObject\\GameObject.h"
#include "StaticMash\StaticMesh.h"


/************************************************************
*	スタティックメッシュオブジェクトクラス.
*	3DSTGではメインの分類.
**/
class StaticMeshObject
	: public GameObject
{
public:
	StaticMeshObject();
	virtual ~StaticMeshObject() override;

	// CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Update() override;
	virtual void Draw() override;

	// メッシュを接続する. (生参照を受け取り、生ポインターに設定)
	void AttachMesh(StaticMesh& pMesh) {
		m_pMesh = &pMesh;
	}
	// スプライトを切り離す.
	void DetachMesh() {
		m_pMesh = nullptr;
	}


protected:
	// 生ポインターに戻すことで、AttachMesh の代入エラーを解消します。
	StaticMesh* m_pMesh;
};