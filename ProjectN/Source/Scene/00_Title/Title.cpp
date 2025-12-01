#include "Title.h"
#include "Sound/SoundManager.h"

Title::Title()
	: SceneBase()
{
	
}

Title::~Title()
{
}

void Title::Initialize()
{
}

void Title::Create()
{
}

void Title::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
        //2点先取チェック：勝敗がついていたら完全リセット
        if (SceneManager::GetInstance()->IsGameFinished())
        {
            SceneManager::GetInstance()->ResetScore();
        }

        //次のゲームへ.
        SceneManager::GetInstance()->LoadScene(SceneManager::Main);
    }
}

void Title::Draw()
{
}