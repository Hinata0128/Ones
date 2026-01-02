#include "EnemyBase.h"

EnemyBase::EnemyBase()
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	Character::Update();
}

void EnemyBase::Draw()
{
	Character::Draw();
}

void EnemyBase::Init()
{
	Character::Init();
}

void EnemyBase::SetTargetPos(const D3DXVECTOR3 Player_Pos)
{
	m_pPlayerPos = Player_Pos;
}

D3DXVECTOR3 EnemyBase::GetTargetPos()
{
	return m_pPlayerPos;
}
