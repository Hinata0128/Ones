#include "LimitTime.h"

#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include "System/02_Singleton/Timer/Timer.h"

LimitTime::LimitTime()
	: UIObject	()

	, m_pClockBackSprite(std::make_shared<Sprite2D>())
	, m_pClockFrameSprite(std::make_shared<Sprite2D>())
	, m_pClockFrontSprite(std::make_shared<Sprite2D>())

	, m_upClockBack(std::make_shared<UIObject>())
	, m_upClockFrame(std::make_shared<UIObject>())
	, m_upClockFront(std::make_shared<UIObject>())
{
	Create();
}

LimitTime::~LimitTime()
{
}

void LimitTime::Create()
{
	//‰æ‘œ‚Ì‚‚³.
	const float Clock = 158.0f;

	const float paddingX = 8.0f;
	const float innerBarW = Clock - (paddingX * 2.0f);

	//§ŒÀŠÔ‚Ì•\¦.
	const float posX = static_cast<float>(WND_W - 250);
	const float posY = 100.0f;

	//§ŒÀŠÔ‚Ìİ’è.
	Sprite2D::SPRITE_STATE ssClockBack = { Clock ,Clock ,Clock ,Clock ,Clock ,Clock };
	m_pClockBackSprite->Init(_T("Data\\Image\\Buttle\\ClockBack.png"), ssClockBack);
	m_upClockBack->AttachSprite(m_pClockBackSprite);
	m_upClockBack->SetPosition(posX, posY, 0.0f);

	Sprite2D::SPRITE_STATE ssClockFrame = { Clock,Clock ,Clock ,Clock ,Clock ,Clock };
	m_pClockFrameSprite->Init(_T("Data\\Image\\Buttle\\ClockFrame.png"), ssClockFrame);
	m_upClockFrame->AttachSprite(m_pClockFrameSprite);
	m_upClockFrame->SetPosition(posX, posY, 0.0f);

	Sprite2D::SPRITE_STATE ssClockFront = { Clock,Clock ,Clock ,Clock ,Clock ,Clock };
	m_pClockFrontSprite->Init(_T("Data\\Image\\Buttle\\ClockFront.png"), ssClockFront);
	m_upClockFront->AttachSprite(m_pClockFrontSprite);
	m_upClockFront->SetPosition(posX, posY, 0.0f);
}

void LimitTime::Update()
{
}

void LimitTime::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	//•\¦.
	m_upClockBack->Draw();
	m_upClockFront->Draw();
	m_upClockFrame->Draw();
	DirectX11::GetInstance()->SetDepth(true);
}
