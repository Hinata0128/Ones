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
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		m_pPortal->Init();
		//EnterƒL[‚ÅMain‚Ö‚Ì‘JˆÚ.
		SceneManager::GetInstance()->LoadScene(SceneManager::Main);
	}
}

void Ending::Draw()
{
}
