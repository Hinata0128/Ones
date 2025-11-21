#include "NomalState.h"

NomalState::NomalState(EnemyNomal* pOwner)
	: StateBase	()
	, m_pOwner	(pOwner)
{
}

NomalState::~NomalState()
{
}

void NomalState::Enter()
{
}

void NomalState::Update()
{
}

void NomalState::Exit()
{
}

void NomalState::Draw()
{
}

void NomalState::Init()
{
}

EnemyNomal* NomalState::GetOwner() const
{
	return nullptr;
}
