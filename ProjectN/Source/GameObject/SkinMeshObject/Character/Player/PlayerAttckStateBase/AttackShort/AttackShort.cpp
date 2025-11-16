#include "AttackShort.h"

#include "..//..//Player.h"
#include "..//..//PlayerContext/PlayerContext.h"
#include "System/02_Singleton/Timer/Timer.h"

AttackShort::AttackShort()
	: PlayerAttckStateBase	()
	, m_ShortCoolDown		(0.0f)
	, m_CoolTime			(0.5f)
{
}

AttackShort::~AttackShort()
{
}

void AttackShort::Enter(Player* player)
{
	m_ShortCoolDown = 0.0f;
}

void AttackShort::ExecuteAttack(Player* player)
{
	//アニメーションの切り替えの実装と、攻撃の実装用の処理.
	constexpr float zero = 0.0f;
	//deltaTimeを取得する.
	float deltaTime = Timer::GetInstance().DeltaTime();

	//クールタイムの処理.
	if (m_CoolTime >= zero)
	{
		m_ShortCoolDown -= deltaTime;
	}

    //弾を撃つ動作.
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && m_ShortCoolDown <= zero)
    {
        // クールタイムリセット
		m_ShortCoolDown = m_CoolTime;
    }
}

void AttackShort::Exit(Player* player)
{
	//終了処理.
}