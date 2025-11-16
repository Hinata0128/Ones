#include "GameObject.h"

//コンストラクタ.
GameObject::GameObject()
	: m_vPosition	{}
	, m_vRotation	( 0.0f, 0.0f, 0.0f )
	, m_vScale		( 1.0f, 1.0f, 1.0f )
{
}

//デストラクタ.
GameObject::~GameObject()
{
}