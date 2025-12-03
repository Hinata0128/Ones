#include "HPBar.h"

#include "Sprite2D//Sprite2D.h"


HPBar::HPBar()
	: UIObject			()
	, m_pHpBarSprite	( std::make_shared<Sprite2D>() )
	, m_pHpBar			( std::make_shared<UIObject>() )
{
	//今はコンストラクタで書いている.
	//場所が違うならDraw()に書く.
	Create();
}

HPBar::~HPBar()
{
}


void HPBar::Update()
{
}

void HPBar::Draw()
{
	//これでHPBarを表示する.
	m_pHpBar->Draw();
}

void HPBar::Create()
{
	const float Size = 64.0;

	//HPBarの構造体.
	Sprite2D::SPRITE_STATE SSHPBar =
	{
		Size * 6,	//横の長さを設定.
		Size,		//縦の長さ.
		Size,		//画像の区切り.		
		Size,		//画像の縦の区切り
		Size,		//表示する画像の幅
		Size		//画像の高さ(黒い枠)
	};

	//HPBarの表示位置の変更.
	const float size = SSHPBar.Disp.w * 0.5f;
	const float pos_y = static_cast<float>(WND_H - Size) - SSHPBar.Disp.h;


	//HPBarの表示.
	m_pHpBarSprite->Init(_T("Data\\Texture\\HPBar.png"), SSHPBar);

	//HPBarのスプライトを表示する.
	m_pHpBar->AttachSprite(m_pHpBarSprite);

	//HPの表示位置を変更する.
	m_pHpBar->SetPosition(size * 0.5f, pos_y, 0.f);
}

