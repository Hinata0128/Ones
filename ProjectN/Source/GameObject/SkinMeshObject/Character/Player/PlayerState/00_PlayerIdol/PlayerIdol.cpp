#include "PlayerIdol.h"

#include "..//..//Player.h"
#include "..//..//PlayerContext/PlayerContext.h"
#include "..//01_PlayerMove//PlayerMove.h"

PlayerIdol::PlayerIdol(Player* pOwner)
    : PlayerState(pOwner)
    //Playerの情報を取得するので引数をpOwnerにしておく.
    , m_pPlayerMove     (std::make_shared<PlayerMove>(pOwner))
{
}

PlayerIdol::~PlayerIdol()
{
}

void PlayerIdol::Enter()
{
}

void PlayerIdol::Execute()
{
    PlayerContext ctx(m_pOwner);

    //PlayerClassとfriendになったのでメンバ変数を設定できるようにした.
    D3DXVECTOR3& Position = ctx.Position;
    D3DXVECTOR3& Rotation = ctx.Rotation;
    int& AnimNo = ctx.AnimNo;
    double& AnimTime = ctx.AnimTime;
    SkinMesh* Mesh = ctx.Mesh;
    LPD3DXANIMATIONCONTROLLER AnimCtrl = ctx.AnimCtrl;
    D3DXVECTOR3& BonePos = ctx.BonePos;

    bool isMoving = false; //移動しているかフラグ.
    const int IDOL_ANIM_NO = 0;


    if (!isMoving)
    {
        //// 現在のアニメーション番号が7番でない場合のみ切り替える
        //if (AnimNo != IDOL_ANIM_NO)
        //{
        //    AnimNo = IDOL_ANIM_NO;
        //    AnimTime = 0.0f; // アニメーション時間をリセット

        //    if (Mesh && AnimCtrl)
        //    {
        //        // アニメーションコントローラに7番をセット
        //        Mesh->ChangeAnimSet(IDOL_ANIM_NO, AnimCtrl);
        //    }
        //}
    }
    //最初はIdolでその中で毎フレームUpdata()が回っている.
    m_pPlayerMove->Execute();
}

void PlayerIdol::Exit()
{
}

void PlayerIdol::Update()
{
    PlayerState::Update();
}

void PlayerIdol::Draw()
{
    PlayerState::Draw();
}

void PlayerIdol::Init()
{
    PlayerState::Init();
}
