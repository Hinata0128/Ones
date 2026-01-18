#include "Ending.h"
#include "Sound/SoundManager.h"

Ending::Ending()
	: SceneBase()
	, m_pPortal			(std::make_unique<Portal>())
{

}

Ending::~Ending()
{
}

void Ending::Initialize()
{
}

void Ending::Create()
{
}

void Ending::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		m_pPortal->Init();
		//EnterƒL[‚ÅMain‚Ö‚Ì‘JˆÚ.
		SceneManager::GetInstance()->LoadScene(SceneManager::OP);
	}
}

void Ending::Draw()
{
}
