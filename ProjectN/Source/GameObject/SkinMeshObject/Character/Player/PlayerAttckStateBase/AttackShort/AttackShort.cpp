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
	m_IsHitActive = false;
	// 当たり判定の半径を設定
	m_SwordHitBox.SetRadius(Radius);
}

void AttackShort::ExecuteAttack(Player* player)
{
	//アニメーションの切り替えの実装と、攻撃の実装用の処理.
	constexpr float zero = 0.0f;
	//deltaTimeを取得する.
	float deltaTime = Timer::GetInstance().DeltaTime();

	m_IsHitActive = false;

	//クールタイムの処理.
	if (m_CoolTime >= zero)
	{
		m_ShortCoolDown -= deltaTime;
	}

    //近距離攻撃動作.
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && m_ShortCoolDown <= zero)
    {
        // クールタイムリセット
		m_ShortCoolDown = m_CoolTime;
		m_IsHitActive = true;
    }
}

void AttackShort::Exit(Player* player)
{
	//終了処理.
}

void AttackShort::Draw(Player* player)
{
	// 1. 描画条件のチェック
	if (player)
	{
		D3DXVECTOR3 bonePos;

		// 2. 剣先 (blade_r_head) のボーン位置 (ワールド座標) を Player 経由で取得
		if (player->GetBonePosition("blade_r_head", &bonePos))
		{
			// 3. BoundingSphere の位置と半径を設定
			m_SwordHitBox.SetPosition(bonePos);
			m_SwordHitBox.SetRadius(Radius);

			// 4. BoundingSphere::Draw() を呼び出してデバッグ描画
			m_SwordHitBox.Draw();
		}
	}
}
