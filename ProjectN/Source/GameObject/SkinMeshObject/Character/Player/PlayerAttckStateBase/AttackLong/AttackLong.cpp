#include "AttackLong.h"

#include "GameObject/SkinMeshObject/Character/Player/Player.h"
#include "System/00_Manager/02_PShotManager/PShotManager.h"
#include "GameObject/SkinMeshObject/Character/Player/PlayerContext/PlayerContext.h"
#include "System/02_Singleton/Timer/Timer.h"

AttackLong::AttackLong()
	: PlayerAttckStateBase  ()
    , m_ShotCoolDown	    (0.0f)
	, m_CoolTime		    (3.0f)
	, m_ShotOffset		    (0.0f, 1.5f, 1.0f)
{
}

AttackLong::~AttackLong()
{
}

void AttackLong::Enter(Player* player)
{
	m_ShotCoolDown = 0.0f;
}

//保存

void AttackLong::ExecuteAttack(Player* player)
{
	constexpr float zero = 0.0f;
	float deltaTime = Timer::GetInstance().DeltaTime();
    PlayerContext ctx(player);

    //ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed);

    //クールタイムの処理.
	if (m_CoolTime >= zero)
	{
		m_ShotCoolDown -= deltaTime;
	}

	//弾を撃つ動作.
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && m_ShotCoolDown <= zero)
	{      

        D3DXMATRIX matS, matR, matT, playerWorldMatrix;

        D3DXMatrixScaling(&matS, ctx.Scale.x, ctx.Scale.y, ctx.Scale.z);
        D3DXMatrixRotationYawPitchRoll(&matR, ctx.Rotation.y, ctx.Rotation.x, ctx.Rotation.z);
        D3DXMatrixTranslation(&matT, ctx.Position.x, ctx.Position.y, ctx.Position.z);

        D3DXMatrixMultiply(&playerWorldMatrix, &matS, &matR);
        D3DXMatrixMultiply(&playerWorldMatrix, &playerWorldMatrix, &matT);

        D3DXVECTOR3 worldBonePos;
        D3DXVec3TransformCoord(&worldBonePos, &ctx.BonePos, &playerWorldMatrix);

        // 弾の発射位置を計算(ワールドボーン位置 + オフセット)
        D3DXVECTOR3 shotPos = worldBonePos + m_ShotOffset;

        // 弾の発射方向を計算
        D3DXVECTOR3 Dir = player->Player_WS(ctx.Rotation.y);
        D3DXVec3Normalize(&Dir, &Dir);

        // Managerに弾を追加
        PShotManager::GetInstance()->AddPlayerShot(shotPos, Dir);

        // クールタイムリセット
        m_ShotCoolDown = m_CoolTime;

    }
}

//void AttackLong::ExecuteAttack(Player* player)
//{
//    constexpr float zero = 0.0f;
//    float deltaTime = Timer::GetInstance().DeltaTime();
//    PlayerContext ctx(player);
//
//    ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed);
//
//    //もし右クリックが押されたとき.
//    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
//    {
//        // アニメーション4再生
//        ctx.AnimNo = 4;
//        ctx.AnimTime = 0.0;
//        ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
//    }
//    // アニメーション経過時間加算
//    ctx.AnimTime += ctx.AnimSpeed;
//    if (ctx.AnimNo == 4 && ctx.AnimTime >= ctx.Mesh->GetAnimPeriod(ctx.AnimNo))
//    {
//        return;
//    }
//    ctx.Mesh->GetPosFromBone("blade_l_head", &ctx.BonePos);
//}


void AttackLong::Exit(Player* player)
{
    //終了処理.
}
