#include "PortalGauge.h"

#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include <GameObject/UIObject/00_HPBar/HPBar.h>

PortalGauge::PortalGauge()
	: UIObject				()
	, m_spBase				(std::make_shared<Sprite2D>())
	, m_spGauge				(std::make_shared<Sprite2D>())

	, m_upPortalGaugeBaseUI	(std::make_shared<UIObject>())
	, m_upPortalGaugeUI		(std::make_shared<UIObject>())

	, m_maxBarScaleX		(1.0f)
	, m_percent				(0.0f)
	, m_barWidth(1.0f)
{
}

PortalGauge::~PortalGauge()
{
}

void PortalGauge::Update()
{
	// --- ゲージの伸縮計算 ---

	// 現在のスケールを計算
	float currentScaleX = m_maxBarScaleX * m_percent;

	// スケール適用
	m_upPortalGaugeUI->SetScale(D3DXVECTOR3(currentScaleX, 1.0f, 1.0f));

	// 【重要】左端固定の計算
	// 通常、SetScaleは中心を基準に伸縮するため、左端が右にズレてしまいます。
	// 左端を固定するために、スケールに応じて座標を補正します。
	// ※Sprite2D側でピボット（原点）を左端に設定できている場合は不要です。
	float offset = (m_barWidth * m_maxBarScaleX * (1.0f - m_percent)) * 0.5f;
	m_upPortalGaugeUI->SetPosition(GetPosition().x - offset, GetPosition().y, 0.0f);

	m_upPortalGaugeBaseUI->Update();
	m_upPortalGaugeUI->Update();
}

void PortalGauge::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	m_upPortalGaugeBaseUI->Draw();
	if (m_percent > 0.0f)
	{
		m_upPortalGaugeUI->Draw();
	}
	DirectX11::GetInstance()->SetDepth(true);
}

void PortalGauge::Create()
{
	//ポータルゲージのベース.
	const float baseW = 1030.0f;
	const float baseH = 55.0f;
	//ゲージの画像サイズ.
	const float barW = 1000.0f; // ゲージ用画像(GaugeBack等)のテクスチャ本来の横幅
	const float barH = 34.0f;   // ゲージ用画像の縦幅

	// バーが枠からはみ出ないための余白設定（左右8ピクセルずつ空ける）
	const float paddingX = 8.0f;
	const float innerBarW = baseW - (paddingX * 2.0f); // 枠の中に入るバーの表示幅

	//表示位置.
	const float posX = 100.0f;
	const float posY = 100.0f;

	m_maxBarScaleX = innerBarW / barW;

	Sprite2D::SPRITE_STATE ssBase = { baseW, baseH, baseW, baseH, baseW, baseH };
	m_spBase->Init(_T("Data\\Image\\Buttle\\BossHP.png"), ssBase);

	m_upPortalGaugeBaseUI->AttachSprite(m_spBase);
	m_upPortalGaugeBaseUI->SetPosition(posX, posY, 0.0f);


	Sprite2D::SPRITE_STATE ssGauge = { barW, barH, barW, barH, barW, barH };
	m_spGauge->Init(_T("Data\\Image\\Buttle\\PortalGauge.png"), ssGauge);

	m_upPortalGaugeUI->AttachSprite(m_spGauge);
	// 最初は背景と同じ位置にセット
	m_upPortalGaugeUI->SetPosition(posX, posY, 0.0f);

	// 自身の位置も保持（管理用）
	SetPosition(posX, posY, 0.0f);
}
