#include "HPBar.h"
#include "Sprite2D//Sprite2D.h"
#include "DirectX/DirectX11.h"

HPBar::HPBar()
	: UIObject()
	, m_spBaseSprite(std::make_shared<Sprite2D>())
	, m_spBackSprite(std::make_shared<Sprite2D>())
	, m_spDamageSprite(std::make_shared<Sprite2D>())
	, m_spGaugeSprite(std::make_shared<Sprite2D>())
	, m_upBase(std::make_shared<UIObject>())
	, m_upBack(std::make_shared<UIObject>())
	, m_upDamage(std::make_shared<UIObject>())
	, m_upGauge(std::make_shared<UIObject>())
{
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
	DirectX11::GetInstance()->SetDepth(false);
	// 描画順が重要（下から順に描画）
	m_upBack->Draw();   // 1. 灰色の背景
	m_upDamage->Draw(); // 2. 赤色のダメージバー
	m_upGauge->Draw();  // 3. 緑色のHPバー
	m_upBase->Draw();   // 4. 一番手前の枠
	DirectX11::GetInstance()->SetDepth(true);
}

void HPBar::Create()
{
	// 各画像の元のサイズ
	const float baseW = 460.0f;
	const float baseH = 64.0f;
	const float barW = 1000.0f;
	const float barH = 34.0f;

	// 調整用パラメータ：枠の左右の縁の幅（仮定値）
	// ※この値を大きくするとバーがより内側に入ります
	const float paddingX = 8.0f;

	// バーが実際に表示されるべき内側の幅を計算
	const float innerBarW = baseW - (paddingX * 2.0f);

	// 表示基準位置
	const float posX = 50.0f;
	const float posY = static_cast<float>(WND_H - 100.0f);

	// 1. HPBase (枠) の設定
	Sprite2D::SPRITE_STATE ssBase = { baseW, baseH, baseW, baseH, baseW, baseH };
	m_spBaseSprite->Init(_T("Data\\Image\\Buttle\\HPBase.png"), ssBase);
	m_upBase->AttachSprite(m_spBaseSprite);
	m_upBase->SetPosition(posX, posY, 0.0f);

	// 2. バーの共通設定と計算
	// 上下中央合わせのためのオフセット
	float offsetY = (baseH - barH) * 0.5f;
	// バーの幅を「内側の幅」に合わせるためのスケールを計算
	float barScaleX = innerBarW / barW;

	Sprite2D::SPRITE_STATE ssBar = { barW, barH, barW, barH, barW, barH };

	// GaugeBack (背景)
	m_spBackSprite->Init(_T("Data\\Image\\Buttle\\GaugeBack.png"), ssBar);
	m_upBack->AttachSprite(m_spBackSprite);
	// X座標を paddingX 分だけ右にずらす
	m_upBack->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upBack->SetScale(D3DXVECTOR3(barScaleX, 1.0f, 1.0f));

	// HPDamage (赤)
	m_spDamageSprite->Init(_T("Data\\Image\\Buttle\\HPDamage.png"), ssBar);
	m_upDamage->AttachSprite(m_spDamageSprite);
	m_upDamage->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upDamage->SetScale(D3DXVECTOR3(barScaleX, 1.0f, 1.0f));

	// HPGauge (緑)
	m_spGaugeSprite->Init(_T("Data\\Image\\Buttle\\HPGauge.png"), ssBar);
	m_upGauge->AttachSprite(m_spGaugeSprite);
	m_upGauge->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upGauge->SetScale(D3DXVECTOR3(barScaleX, 1.0f, 1.0f));
}