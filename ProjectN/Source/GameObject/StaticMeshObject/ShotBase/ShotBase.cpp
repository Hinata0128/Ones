#include "ShotBase.h"

ShotBase::ShotBase()
	: m_Speed(0.02f)
	, m_Disp(false)
	, m_IsActive(false)
	// BoundingSphere の実体化は shared_ptr で正しく行われています
	, m_BSphere(std::make_shared<BoundingSphere>())
{
}

ShotBase::~ShotBase()
{
}

void ShotBase::Update()
{
	// 必要なら共通の弾移動処理をここに書く（現状は子クラス任せ）
	StaticMeshObject::Update();
}

void ShotBase::Draw()
{
	// 表示中のみ描画
	if (m_Disp)
	{
		StaticMeshObject::Draw();
	}
}

void ShotBase::Init()
{
	m_vPosition = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Disp = false;
	m_IsActive = false;

	if (m_BSphere)
	{
		m_BSphere->SetPosition(m_vPosition);
	}
}

void ShotBase::UpdateBPosition()
{
	if (m_BSphere)
	{
		m_BSphere->SetPosition(m_vPosition);
	}
}

BoundingSphere& ShotBase::GetBoundingSphere() const
{
	// shared_ptr の中身（実体）への参照を返す
	return *m_BSphere;
}