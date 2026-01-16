#include "TestFont.h"
#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"

TestFont::TestFont()
	: UIObject()
	, m_FontSprite_0(std::make_shared<Sprite2D>())
	, m_FontSprite_1(std::make_shared<Sprite2D>())

	, m_FontUI_0(std::make_shared<UIObject>())
	, m_FontUI_1(std::make_shared<UIObject>())
{
	Create();
}

TestFont::~TestFont()
{
}

void TestFont::Create()
{
	const float charSize = 32.0f;
	const float texW = 32.0f;
	const float texH = 32.0f;

	Sprite2D::SPRITE_STATE ssFont =
	{
		{ charSize, charSize }, // Disp
		{ texW,     texH     }, // Base
		{ charSize, charSize }  // Stride
	};

	const float posX = 50.0f;
	const float posY = static_cast<float>(WND_H - 150.0f);

	m_FontSprite_0->Init(_T("Data\\Image\\Setting\\PlayerHPFont_0.png"), ssFont);
	m_FontUI_0->AttachSprite(m_FontSprite_0);
	m_FontUI_0->SetPosition(posX, posY, 0.0f);
	//•F‚Ì˜g.
	m_FontSprite_1->Init(_T("Data\\Image\\Setting\\PlayerHPFont_1.png"), ssFont);
	m_FontUI_1->AttachSprite(m_FontSprite_1);
	m_FontUI_1->SetPosition(posX + 32, posY, 0.0f);


}

void TestFont::Update()
{
}

void TestFont::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	m_FontUI_0->Draw();
	m_FontUI_1->Draw();
	DirectX11::GetInstance()->SetDepth(true);
}
