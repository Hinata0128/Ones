#include "HPBar.h"
#include "Sprite2D/Sprite2D.h"
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
	// 1. ターゲット（Player）がセットされているか確認
	if (!m_pTargetPlayer) return;

	// 2. Playerから現在のHPを取得
	float currentHP = m_pTargetPlayer->GetHitPoint();
	float maxHP = 100.0f; // Player::Init()で100.0fに設定されているため

	// 3. HPの割合（0.0 ～ 1.0）を計算
	float hpRate = currentHP / maxHP;
	if (hpRate < 0.0f) hpRate = 0.0f;

	// 4. バーの横幅を更新
	// m_maxBarScaleX（満タン時の幅）に対して hpRate を掛ける
	m_upGauge->SetScale(D3DXVECTOR3(m_maxBarScaleX * hpRate, 1.0f, 1.0f));

	// 赤色（ダメージ）バーも一旦同じように動かす
	// ※ここを徐々に減らすようにするとダメージ演出になります
	m_upDamage->SetScale(D3DXVECTOR3(m_maxBarScaleX * hpRate, 1.0f, 1.0f));
}

void HPBar::Draw()
{
	// UIが背景より前に来るように深度バッファを無効化
	DirectX11::GetInstance()->SetDepth(false);

	m_upBack->Draw();   // 灰色背景
	m_upDamage->Draw(); // 赤色バー
	m_upGauge->Draw();  // 緑色バー
	m_upBase->Draw();   // 外枠

	DirectX11::GetInstance()->SetDepth(true);
}

void HPBar::Create()
{
	// 各画像のサイズ定義
	const float baseW = 460.0f;
	const float baseH = 64.0f;
	const float barW = 1000.0f;
	const float barH = 34.0f;

	// バーが枠からはみ出ないための余白設定
	const float paddingX = 8.0f;
	const float innerBarW = baseW - (paddingX * 2.0f);

	// 表示位置
	const float posX = 50.0f;
	const float posY = static_cast<float>(WND_H - 100.0f);

	// HP満タン時のスケールを計算して保持
	m_maxBarScaleX = innerBarW / barW;

	// 1. HPBase (枠) の設定
	Sprite2D::SPRITE_STATE ssBase = { baseW, baseH, baseW, baseH, baseW, baseH };
	m_spBaseSprite->Init(_T("Data\\Image\\Buttle\\HPBase.png"), ssBase);
	m_upBase->AttachSprite(m_spBaseSprite);
	m_upBase->SetPosition(posX, posY, 0.0f);

	// 2. バーの共通設定
	float offsetY = (baseH - barH) * 0.5f;
	Sprite2D::SPRITE_STATE ssBar = { barW, barH, barW, barH, barW, barH };

	// 背景（灰色）
	m_spBackSprite->Init(_T("Data\\Image\\Buttle\\GaugeBack.png"), ssBar);
	m_upBack->AttachSprite(m_spBackSprite);
	m_upBack->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upBack->SetScale(D3DXVECTOR3(m_maxBarScaleX, 1.0f, 1.0f));

	// ダメージバー（赤）
	m_spDamageSprite->Init(_T("Data\\Image\\Buttle\\HPDamage.png"), ssBar);
	m_upDamage->AttachSprite(m_spDamageSprite);
	m_upDamage->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upDamage->SetScale(D3DXVECTOR3(m_maxBarScaleX, 1.0f, 1.0f));

	// HPバー（緑）
	m_spGaugeSprite->Init(_T("Data\\Image\\Buttle\\HPGauge.png"), ssBar);
	m_upGauge->AttachSprite(m_spGaugeSprite);
	m_upGauge->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upGauge->SetScale(D3DXVECTOR3(m_maxBarScaleX, 1.0f, 1.0f));
}