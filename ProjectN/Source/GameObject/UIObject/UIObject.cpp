#include "UIObject.h"

UIObject::UIObject()
	: m_pSprite		( nullptr )
	, m_PatternNo	()
{
}

UIObject::~UIObject()
{
	DetachSprite();
}

void UIObject::Update()
{
	if( m_pSprite == nullptr ){
		return;
	}
}

//引数なしのDraw()関数
void UIObject::Draw()
{
	if (m_pSprite == nullptr) {
		return;
	}

	// 描画直前で座標・回転・スケールを反映
	m_pSprite->SetPosition(m_vPosition);
	m_pSprite->SetRotation(m_vRotation);
	m_pSprite->SetScale(m_vScale);

	// パターン番号設定
	m_pSprite->SetPatternNo(m_PatternNo.x, m_PatternNo.y);

	// レンダリング
	m_pSprite->Render();
}

