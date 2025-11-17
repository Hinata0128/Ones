#include "PlayerMove.h"

#include "..//..//Player.h"
#include "..//..//PlayerContext/PlayerContext.h"

//ローカル変数.
constexpr float zero = 0.0f;
constexpr float add_value = 0.1f;

//テストbranch作成しました

PlayerMove::PlayerMove(Player* pOwner)
    : PlayerState(pOwner)
    , FastAnim  (false)
    , step      (enStep::none)
    , LStep     (enLeftStep::none)
    , Move      (enMove::Idol)
{
    //初期化を書いている.
    Init();
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Enter()
{
    PlayerState::Enter();
}

void PlayerMove::Execute()
{
    PlayerContext ctx(m_pOwner);

    float deltaTime = Timer::GetInstance().DeltaTime();
    //右クリックの攻撃の関数.
    bool RAttacking = RbuttonAttackStep(ctx);
    bool LAttacking = LButtonAttackStep(ctx);
    //WASDの入力取得.
    Move = GetMoveInput();

    //PlayerのWASDの動作をさせるための関数.
    D3DXVECTOR3 ForwardAndBackward = m_pOwner->Player_WS(ctx.Rotation.y);
    D3DXVECTOR3 LeftAndRight = m_pOwner->Player_AD(ctx.Rotation.y);

    //移動処理.
    //第一引数にPlayerContext.
    //第二引数にWSの関数.
    //第三引数にADの関数.
    HandleMove(ctx, ForwardAndBackward, LeftAndRight);

    //ステートの共通処理.
    PlayerState::Execute();
}


void PlayerMove::Exit()
{
    PlayerState::Exit();
}

void PlayerMove::Update()
{
    PlayerState::Update();
}

void PlayerMove::Draw()
{
    PlayerState::Draw();
}

void PlayerMove::Init()
{

    PlayerState::Init();
}

//型をboolに変更させたのでbreckではなくreturn false/trueで返す.
bool PlayerMove::RbuttonAttackStep(PlayerContext& ctx)
{
    // 右クリック押されたら初期ステップへ
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        if (step == enStep::none)
        {
            step = enStep::first;
        }
    }

    switch (step) 
    {
        case enStep::none:
            return false; //攻撃していない→移動処理に移動.
        case enStep::first:
            //アニメーション切り替え.
            ctx.AnimNo = 8; //アニメーション番号.
            ctx.AnimTime = 0.0f;    //アニメーションタイマーの初期化.
            ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed, ctx.AnimCtrl);
            ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);//アニメーションの変更.
            step = enStep::run;
            return true;
        case enStep::run:
        {
            float period = ctx.Mesh->GetAnimPeriod(6);
            if (ctx.AnimTime > period)
            {
                step = enStep::end;
            }
            else
            {
                ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed, ctx.AnimCtrl);
                ctx.AnimTime += ctx.AnimSpeed;
            }
            return true;
        }
        case enStep::end:
            ctx.Mesh->SetAnimSpeed(0.0f, ctx.AnimCtrl);
            m_pOwner->ChangeAttackType(PlayerAttackManager::enAttack::Long);

            if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
            {
                step = enStep::release_anim;
            }
            return true;
        case enStep::release_anim:
        {
            float period = ctx.Mesh->GetAnimPeriod(6);
            if (ctx.AnimTime >= period)
            {
                ctx.AnimNo = 0;
                ctx.AnimTime = 0.0f;
                ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
                step = enStep::none;
            }
            return true;
        }
    }
    return false;
}

//左クリックを押したときの近距離攻撃.
bool PlayerMove::LButtonAttackStep(PlayerContext& ctx)
{
    //左クリックを押したら次のステップに入る.
    if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
    {
        if (LStep == enLeftStep::none)
        {
            LStep = enLeftStep::first;
        }
    }

    switch (LStep)
    {
    case enLeftStep::none:
        return false; //攻撃していない→移動処理に移動.
    case enLeftStep::first:
        //アニメーション切り替え.
        ctx.AnimNo = 6; //アニメーション番号.
        ctx.AnimTime = 0.0f;    //アニメーションタイマーの初期化.
        ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);//アニメーションの変更.
        LStep = enLeftStep::run;
        return true;
    case enLeftStep::run:
    {
        float period = ctx.Mesh->GetAnimPeriod(12);
        if (ctx.AnimTime > period)
        {
            LStep = enLeftStep::end;
        }
        else
        {
            ctx.AnimTime += ctx.AnimSpeed;
        }
        return true;
    }
    case enLeftStep::end:
        ctx.Mesh->SetAnimSpeed(0.0f, ctx.AnimCtrl);
        m_pOwner->ChangeAttackType(PlayerAttackManager::enAttack::Short);

        LStep = enLeftStep::release_anim;
        return true;
    case enLeftStep::release_anim:
    {
        float period = ctx.Mesh->GetAnimPeriod(6);
        if (ctx.AnimTime >= period)
        {
            ctx.AnimNo = 0;
            ctx.AnimTime = 0.0f;
            ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
            LStep = enLeftStep::none;
        }
        return true;
    }
    }
    return false;
}

void PlayerMove::HandleMove(
    PlayerContext& ctx,
    const D3DXVECTOR3& ForwardAndBackward,
    const D3DXVECTOR3& LeftAndRight)
{
    // 攻撃中かどうか（アニメを変更するか判断）
    bool IsRAttacking = (step != enStep::none);
    bool IsLAttacking = (LStep != enLeftStep::none);
    //移動アニメーションを適応させる.
    auto ApplyMoveAnimation = [&](int animNo)
    {
        if (IsRAttacking || IsLAttacking)
        {
            // 攻撃中はアニメ変更禁止
            return;
        }
        if (ctx.AnimNo != animNo)
        {
            ctx.AnimNo = animNo;    //アニメーション番号.
            ctx.AnimTime = 0.0;     //アニメーションタイマー初期化.
            ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed, ctx.AnimCtrl);    //アニメーションの速度.
            ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);      //アニメーションの変更.
        }
    };
    switch (Move)
    {
        //原因の解決をするけれども今ctx.AnimTime += ctx.AnimSpeedを書いていると丁度でいいところでアニメーションが停止する.
        case enMove::Idol:
        {
            ctx.AnimTime += ctx.AnimSpeed;
        }
        break;
        //前進.
        case enMove::ForWard:
        {
            ctx.Position += ForwardAndBackward * add_value;
            ApplyMoveAnimation(2);
            // アニメーション進行
            ctx.AnimTime += ctx.AnimSpeed;
            // 最後まで再生されたかチェック
            float period = ctx.Mesh->GetAnimPeriod(12);
            if (ctx.AnimTime >= period)
            {
                ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl);
                // アニメーション進行
                ctx.AnimTime += ctx.AnimSpeed;
            }
        }
        break;
        //後退.
        case enMove::Back:
        {
            ctx.Position -= ForwardAndBackward * add_value;
            ApplyMoveAnimation(0);
            // アニメーション進行
            ctx.AnimTime += ctx.AnimSpeed;
        }
        break;
        //左.
        case enMove::Left:
        {
            ctx.Position -= LeftAndRight * add_value;
            ApplyMoveAnimation(2);
            // アニメーション進行
            ctx.AnimTime += ctx.AnimSpeed;
            // 最後まで再生されたかチェック
            float period = ctx.Mesh->GetAnimPeriod(12);
            if (ctx.AnimTime >= period)
            {
                ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl);
                ctx.AnimTime += ctx.AnimSpeed;
            }
        }
        break;
        //右.
        case enMove::Right:
        {
            ctx.Position += LeftAndRight * add_value;
            ApplyMoveAnimation(2);
            // アニメーション進行
            ctx.AnimTime += ctx.AnimSpeed;
            // 最後まで再生されたかチェック
            float period = ctx.Mesh->GetAnimPeriod(12);
            if (ctx.AnimTime >= period)
            {
                ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl);
                ctx.AnimTime += ctx.AnimSpeed;
            }
        }
        break;
        //左斜め前.
        case enMove::ForWardAndLeft:
        {
            ctx.Position += ForwardAndBackward * add_value;
            ctx.Position -= LeftAndRight * add_value;
            ApplyMoveAnimation(2);
            // アニメーション進行
            ctx.AnimTime += ctx.AnimSpeed;
            // 最後まで再生されたかチェック
            float period = ctx.Mesh->GetAnimPeriod(12);
            if (ctx.AnimTime >= period)
            {
                ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl);
            }
        }
        break;
        //右斜め前.
        case enMove::ForWardAAndRight:
        {
            ctx.Position += ForwardAndBackward * add_value;
            ctx.Position += LeftAndRight * add_value;
            ApplyMoveAnimation(2);
            // アニメーション進行
            ctx.AnimTime += ctx.AnimSpeed;
            // 最後まで再生されたかチェック
            float period = ctx.Mesh->GetAnimPeriod(12);
            if (ctx.AnimTime >= period)
            {
                ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl);
            }
        }
        break;
        //左後ろ.
        case enMove::BackAndLeft:
        {
            ctx.Position -= ForwardAndBackward * add_value;
            ctx.Position -= LeftAndRight * add_value;
            ApplyMoveAnimation(0);
            ctx.AnimTime += ctx.AnimSpeed;
        }
        break;
        //右後ろ.
        case enMove::BackAndRight:
        {
            ctx.Position -= ForwardAndBackward * add_value;
            ctx.Position += LeftAndRight * add_value;
            ApplyMoveAnimation(0);            
            ctx.AnimTime += ctx.AnimSpeed;
        }
        break;
        default:
        break;
    }
}

//Playerの動作[GetAsyncKeyState()]を書く関数.
PlayerMove::enMove PlayerMove::GetMoveInput()
{
    //WASDの動作.
    bool W = (GetAsyncKeyState('W') & 0x8000);
    bool A = (GetAsyncKeyState('A') & 0x8000);
    bool S = (GetAsyncKeyState('S') & 0x8000);
    bool D = (GetAsyncKeyState('D') & 0x8000);

    //斜め優先判定.
    if (W && D) return enMove::ForWardAAndRight;
    if (W && A) return enMove::ForWardAndLeft;
    if (S && D) return enMove::BackAndRight;
    if (S && A) return enMove::BackAndLeft;

    //単方向判定.
    if (W) return enMove::ForWard;
    if (S) return enMove::Back;
    if (A) return enMove::Left;
    if (D) return enMove::Right;

    //どのキーも押していないときは、0番目のアニメーションを再生.
    return enMove::Idol;
}
