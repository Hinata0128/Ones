#include "SpriteObject.h"

SpriteObject::SpriteObject()
	: m_pSprite(nullptr)
{
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
}

SpriteObject::~SpriteObject()
{
	DetachSprite();
}

void SpriteObject::Update()
{
	if (m_pSprite == nullptr) {
		return;
	}
	// 必要ならここに更新処理を追加
}

void SpriteObject::Draw()
{
	if (m_pSprite == nullptr) {
		return;
	}

	// 座標や回転情報などをスプライトに反映
	m_pSprite->SetPosition(m_vPosition);
	m_pSprite->SetRotation(m_vRotation);
	m_pSprite->SetScale(m_vScale);

	// 設定済みのView・Proj行列を使用して描画
	m_pSprite->Render(m_View, m_Proj);
}
