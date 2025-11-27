#include "HPBar.h"

#include "Sprite2D//Sprite2D.h"


HPBar::HPBar()
	: UIObject			()
	, m_pHpBarSprite	( nullptr )
	, m_pHpBar			( std::make_shared<UIObject>() )
{
}

HPBar::~HPBar()
{
	//破棄.
	SAFE_DELETE( m_pHpBarSprite );
}


void HPBar::Update()
{
}

void HPBar::Draw()
{
	const float Size = 64.0;
	
	m_pHpBarSprite = new Sprite2D();

	//HPBarの構造体.
	Sprite2D::SPRITE_STATE SSHPBar =
	{ 
		Size * 6,	//横の長さを設定.
		Size,
		Size, 
		Size, 
		Size, 
		Size 
	};

	//HPBarの表示位置の変更.
	const float size = SSHPBar.Disp.w * 0.5f;
	const float pos_y = static_cast<float>(WND_H - Size) - SSHPBar.Disp.h;


	//HPBarの表示.
	m_pHpBarSprite->Init(_T("Data\\Texture\\HPBar.png"), SSHPBar);

	//HPBarのスプライトを表示する.
	m_pHpBar->AttachSprite(*m_pHpBarSprite);

	//HPの表示位置を変更する.
	m_pHpBar->SetPosition(size * 0.5f, pos_y, 0.f);

	//これでHPBarを表示する.
	m_pHpBar->Draw();
}

