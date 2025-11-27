#include "NomalDead.h"

#include "..//..//EnemyNomal.h"
#include "..//..//NomalContext/NomalContext.h"

NomalDead::NomalDead(EnemyNomal* pOwner)
	: NomalState	(pOwner)
{
}

NomalDead::~NomalDead()
{
}

void NomalDead::Enter()
{
	NomalContext ctx(m_pOwner);

	ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed);

	ctx.AnimNo = 1;
	ctx.AnimTime = 0.0;
	ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);

	NomalState::Enter();
}

void NomalDead::Update()
{
	NomalState::Update();
}

void NomalDead::Exit()
{
	NomalState::Exit();
}

void NomalDead::Draw()
{
	NomalState::Draw();
}

void NomalDead::Init()
{
	NomalState::Init();
}
