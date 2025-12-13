#include "NomalIdol.h"

#include "..//..//Boss.h"
#include "..//..//NomalContext//NomalContext.h"
#include "..//01_NomalMove/NomalMove.h"

#include "System/02_Singleton/Timer/Timer.h"

NomalIdol::NomalIdol(Boss* pOwner)
	: NomalState	(pOwner)
{
}

NomalIdol::~NomalIdol()
{
}

void NomalIdol::Enter()
{
}

void NomalIdol::Update()
{
    NomalContext ctx(m_pOwner);

    const int IDLE_ANIM_NO = 2;

    // アニメ番号が違うときだけセット（1回だけ実行される）
    if (ctx.AnimNo != IDLE_ANIM_NO)
    {
        ctx.AnimNo = IDLE_ANIM_NO;
        ctx.AnimTime = 0.0f;

        ctx.Mesh->ChangeAnimSet(IDLE_ANIM_NO, ctx.AnimCtrl);
    }

    // アニメーションを前進
    float delta = Timer::GetInstance().DeltaTime();
    ctx.AnimTime += ctx.AnimSpeed * delta;

    // 共通更新
    NomalState::Update();

    // 距離で移動ステートへ遷移
    Boss* pEnemy = dynamic_cast<Boss*>(m_pOwner);

    D3DXVECTOR3 diff = pEnemy->GetPlayerPos() - pEnemy->GetPosition();
    float distanceSq = D3DXVec3LengthSq(&diff);

    const float changeDistance = 15.0f * 15.0f;

    if (distanceSq < changeDistance)
    {
        pEnemy->ChangeState(pEnemy->m_pMove.get());
        return;
    }
}

void NomalIdol::Exit()
{
	NomalState::Exit();
}

void NomalIdol::Draw()
{
	NomalState::Draw();
}

void NomalIdol::Init()
{
	NomalState::Init();
}
