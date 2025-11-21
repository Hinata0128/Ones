#include "NomlIdol.h"

#include "..//..//EnemyNomal.h"
#include "..//..//NomalContext//NomalContext.h"

NomlIdol::NomlIdol(EnemyNomal* pOwner)
	: NomalState	(pOwner)
{
}

NomlIdol::~NomlIdol()
{
}

void NomlIdol::Enter()
{
}

void NomlIdol::Update()
{
//EnemyのIdol状態の処理.
#if 0
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
	NomalState::Update();
#endif
}

void NomlIdol::Exit()
{
	NomalState::Exit();
}

void NomlIdol::Draw()
{
	NomalState::Draw();
}

void NomlIdol::Init()
{
	NomalState::Init();
}
