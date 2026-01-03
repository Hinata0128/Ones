#include "HPBar.h"
#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include <algorithm> // std::max, std::min用

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
	// ここで呼ぶとまだ画像パスなどの準備ができていない場合があるため、
	// 通常はGameMainなどのCreateから呼ぶのが安全ですが、
	// コンストラクタで呼ぶ仕様を維持します。
	Create();
}

HPBar::~HPBar()
{
}

void HPBar::Update()
{
	// ターゲット（Player）が設定されていない場合は何もしない
	if (!m_pTargetPlayer) return;

	// -----------------------------------------------------------
	// 1. 現在のHPから「目標となるスケール」を計算
	// -----------------------------------------------------------
	// 最大HP100と仮定して割合(0.0～1.0)を算出
	float hpRate = m_pTargetPlayer->GetHitPoint() / 100.0f;
	hpRate = max(0.0f, min(1.0f, hpRate)); // 0～1の範囲にクランプ

	// 本来の最大幅（m_maxBarScaleX）に現在のHP率を掛ける
	float targetScaleX = m_maxBarScaleX * hpRate;

	// -----------------------------------------------------------
	// 2. 緑色のゲージ（HPGauge）：即座に目標値にする
	// -----------------------------------------------------------
	if (m_upGauge)
	{
		D3DXVECTOR3 greenScale = m_upGauge->GetScale();
		greenScale.x = targetScaleX;
		m_upGauge->SetScale(greenScale);
	}

	// -----------------------------------------------------------
	// 3. 赤色のゲージ（HPDamage）：じわじわ追いかける
	// -----------------------------------------------------------
	if (m_upDamage)
	{
		D3DXVECTOR3 redScale = m_upDamage->GetScale();

		// 線形補間(Lerp)の計算
		// (目標値 - 現在の値) * 速度。 0.05f を小さくするとよりゆっくり消えます。
		float interpolationSpeed = 0.05f;
		redScale.x += (targetScaleX - redScale.x) * interpolationSpeed;

		// 赤が緑より小さくならないように補正（計算誤差対策）
		if (redScale.x < targetScaleX) {
			redScale.x = targetScaleX;
		}

		m_upDamage->SetScale(redScale);
	}

	// 各UIパーツのUpdateを呼ぶ（アニメーション等がある場合用）
	if (m_upBase)   m_upBase->Update();
	if (m_upBack)   m_upBack->Update();
	if (m_upDamage) m_upDamage->Update();
	if (m_upGauge)  m_upGauge->Update();
}

void HPBar::Draw()
{
	// UIが3Dモデル（背景など）に隠れないように深度バッファを一時的に無効化
	DirectX11::GetInstance()->SetDepth(false);

	// 重なり順：下から順に描画
	if (m_upBack)   m_upBack->Draw();   // 1. 灰色背景（一番下）
	if (m_upDamage) m_upDamage->Draw(); // 2. 赤色バー（ダメージ跡）
	if (m_upGauge)  m_upGauge->Draw();  // 3. 緑色バー（現在のHP）
	if (m_upBase)   m_upBase->Draw();   // 4. 外枠（一番上：バーの端を隠す）

	// 深度バッファを有効に戻す
	DirectX11::GetInstance()->SetDepth(true);
}

void HPBar::Create()
{
	// -----------------------------------------------------------
	// 各画像のサイズ定義（ピクセル単位）
	// -----------------------------------------------------------
	const float baseW = 460.0f;  // HPBase.png の横幅
	const float baseH = 64.0f;   // HPBase.png の縦幅
	const float barW = 1000.0f; // ゲージ用画像(GaugeBack等)のテクスチャ本来の横幅
	const float barH = 34.0f;   // ゲージ用画像の縦幅

	// バーが枠からはみ出ないための余白設定（左右8ピクセルずつ空ける）
	const float paddingX = 8.0f;
	const float innerBarW = baseW - (paddingX * 2.0f); // 枠の中に入るバーの表示幅

	// 表示位置（画面左下付近）
	const float posX = 50.0f;
	const float posY = static_cast<float>(WND_H - 100.0f);

	// -----------------------------------------------------------
	// スケールの計算
	// -----------------------------------------------------------
	// 画像の元のサイズ(barW)に対して、表示したい幅(innerBarW)の比率を求める
	// これにより、テクスチャが枠にピッタリ収まります。
	m_maxBarScaleX = innerBarW / barW;

	// -----------------------------------------------------------
	// 1. HPBase (外枠) の設定
	// -----------------------------------------------------------
	Sprite2D::SPRITE_STATE ssBase = { baseW, baseH, baseW, baseH, baseW, baseH };
	m_spBaseSprite->Init(_T("Data\\Image\\Buttle\\HPBase.png"), ssBase);
	m_upBase->AttachSprite(m_spBaseSprite);
	m_upBase->SetPosition(posX, posY, 0.0f);

	// -----------------------------------------------------------
	// 2. 各バーの共通設定
	// -----------------------------------------------------------
	// 枠の高さ(baseH)とバーの高さ(barH)の差から、上下中央に来るオフセットを計算
	float offsetY = (baseH - barH) * 0.5f;
	Sprite2D::SPRITE_STATE ssBar = { barW, barH, barW, barH, barW, barH };
	D3DXVECTOR3 initialScale(m_maxBarScaleX, 1.0f, 1.0f);

	// 背景（灰色ゲージ）
	m_spBackSprite->Init(_T("Data\\Image\\Buttle\\GaugeBack.png"), ssBar);
	m_upBack->AttachSprite(m_spBackSprite);
	m_upBack->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upBack->SetScale(initialScale);

	// ダメージバー（赤色ゲージ）
	m_spDamageSprite->Init(_T("Data\\Image\\Buttle\\HPDamage.png"), ssBar);
	m_upDamage->AttachSprite(m_spDamageSprite);
	m_upDamage->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upDamage->SetScale(initialScale);

	// メインバー（緑色ゲージ）
	m_spGaugeSprite->Init(_T("Data\\Image\\Buttle\\HPGauge.png"), ssBar);
	m_upGauge->AttachSprite(m_spGaugeSprite);
	m_upGauge->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upGauge->SetScale(initialScale);

	// ※以前ここで m_maxBarScaleX = 1.0f; と上書きしていたため巨大化していました。
	// その一行を削除し、計算した正しい比率を保持したまま終了します。
}