#include "NomalIdol.h"

#include "..//..//EnemyNomal.h"
#include "..//..//NomalContext//NomalContext.h"
#include "..//01_NomalMove/NomalMove.h"

NomalIdol::NomalIdol(EnemyNomal* pOwner)
	: NomalState	(pOwner)
	, m_pMove		(std::make_shared<NomalMove>(pOwner))
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
//EnemyのIdol状態の処理.
#if 1
	NomalContext ctx(m_pOwner);

	//今は隠しておく.
	//移動しているかフラグ.
	bool IsMoving = false;
	//待機のアニメーションの変数.
	const int Idol_Anim_No = 0;

	if (!IsMoving)
	{
		//現在のアニメーション番号が0番でない場合のみ切り替える.
		if (ctx.AnimNo != Idol_Anim_No)
		{
			ctx.AnimNo = Idol_Anim_No;
			//アニメション時間をリセット.
			ctx.AnimTime = 0.0;

			if (ctx.Mesh && ctx.AnimCtrl)
			{
				//アニメションコントローラーに0番をセット.
				ctx.Mesh->ChangeAnimSet(Idol_Anim_No, ctx.AnimCtrl);
			}
		}
	}
	m_pMove->Update();

	NomalState::Update();
#endif
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
