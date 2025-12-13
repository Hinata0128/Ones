#include "NomalState.h"
#include "..//Boss.h"

NomalState::NomalState(Boss* pOwner)
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

Boss* NomalState::GetOwner() const
{
	return m_pOwner;
}
