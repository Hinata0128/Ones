#include "Ending.h"
#include "Sound/SoundManager.h"

Ending::Ending()
	: CSceneBase		()
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
		//EnterƒL[‚ÅMain‚Ö‚Ì‘JˆÚ.
		SceneManager::GetInstance()->LoadScene(SceneManager::Main);
	}
}

void Ending::Draw()
{
}
