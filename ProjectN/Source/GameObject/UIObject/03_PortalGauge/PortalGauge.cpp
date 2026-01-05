#include "PortalGauge.h"
#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include <algorithm>

PortalGauge::PortalGauge()
	: UIObject()
	, m_spBaseSprite(std::make_shared<Sprite2D>())
	, m_spGaugeSprite(std::make_shared<Sprite2D>())
	, m_upBase(std::make_shared<UIObject>())
	, m_upGauge(std::make_shared<UIObject>())
	, m_maxBarScaleX(0.0f)
	, m_percent(0.0f)
	, m_currentScaleX(0.0f)
	, m_barWidth(0.0f)
{
	Create();
}

PortalGauge::~PortalGauge()
{
}

void PortalGauge::Create()
{
	// -----------------------------------------------------------
	// 1. 各画像のサイズ定義（HPBarと数値を完全に合わせる）
	// -----------------------------------------------------------
	const float baseW = 460.0f;  // HPBase.png の横幅
	const float baseH = 64.0f;   // HPBase.png の縦幅
	const float barW = 1000.0f; // PortalGauge.png の本来の横幅
	const float barH = 34.0f;   // PortalGauge.png の本来の縦幅

	const float paddingX = 8.0f; // HPBarと同じ余白
	const float innerBarW = baseW - (paddingX * 2.0f);

	// 表示位置 (HPBarがposX=50なので、PortalGaugeは画面上の適切な位置に)
	const float posX = 512.0f;
	const float posY = 100.0f;

	m_barWidth = barW;
	m_maxBarScaleX = innerBarW / barW; // 枠にピッタリ収まる倍率
	SetPosition(posX, posY, 0.0f);

	// -----------------------------------------------------------
	// 2. 外枠 (Base) の設定
	// -----------------------------------------------------------
	Sprite2D::SPRITE_STATE ssBase = { baseW, baseH, baseW, baseH, baseW, baseH };
	m_spBaseSprite->Init(_T("Data\\Image\\Buttle\\HPBase.png"), ssBase);
	m_upBase->AttachSprite(m_spBaseSprite);
	m_upBase->SetPosition(posX, posY, 0.0f);

	// -----------------------------------------------------------
	// 3. ゲージ (Gauge) の設定
	// -----------------------------------------------------------
	// HPBarと同じく、上下中央に来るオフセットを計算
	float offsetY = (baseH - barH) * 0.5f;
	Sprite2D::SPRITE_STATE ssBar = { barW, barH, barW, barH, barW, barH };

	m_spGaugeSprite->Init(_T("Data\\Image\\Buttle\\PortalGauge.png"), ssBar);
	m_upGauge->AttachSprite(m_spGaugeSprite);

	// 【重要】HPBarと同じく、posX + paddingX で座標を一度だけ固定する
	m_upGauge->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upGauge->SetScale(D3DXVECTOR3(0.0f, 1.0f, 1.0f));
}

void PortalGauge::Update()
{
	// -----------------------------------------------------------
	// 1. 滑らかな動きの計算 (HPBarのLerpと同じ)
	// -----------------------------------------------------------
	float targetScaleX = m_maxBarScaleX * m_percent;

	// interpolationSpeedを0.02fにするとHPBarと同じ速度になります
	float interpolationSpeed = 0.05f;
	m_currentScaleX += (targetScaleX - m_currentScaleX) * interpolationSpeed;

	// -----------------------------------------------------------
	// 2. スケールの適用のみ行う (座標計算はCreateで行ったものを使用)
	// -----------------------------------------------------------
	if (m_upGauge)
	{
		// HPBarのUpdateと同じく、座標（SetPosition）は動かさずScaleだけ変える
		m_upGauge->SetScale(D3DXVECTOR3(m_currentScaleX, 1.0f, 1.0f));
	}

	// 各UIパーツのUpdateを呼ぶ
	if (m_upBase) { m_upBase->Update(); }
	if (m_upGauge) { m_upGauge->Update(); }
}

void PortalGauge::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);

	// HPBarのDrawと同じ重なり順：中身(下) -> 枠(上)
	if (m_upGauge && m_currentScaleX > 0.001f)
	{
		m_upGauge->Draw();
	}
	if (m_upBase)
	{
		m_upBase->Draw();
	}

	DirectX11::GetInstance()->SetDepth(true);
}