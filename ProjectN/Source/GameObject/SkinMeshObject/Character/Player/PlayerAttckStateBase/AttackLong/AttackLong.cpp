#include "AttackLong.h"

#include "GameObject/SkinMeshObject/Character/Player/Player.h"
#include "System/00_Manager/02_PShotManager/PShotManager.h"
#include "GameObject/SkinMeshObject/Character/Player/PlayerContext/PlayerContext.h"
#include "System/02_Singleton/Timer/Timer.h"

#include "System/00_Manager/03_ImGuiManager/ImGuiManager.h"

AttackLong::AttackLong()
	: PlayerAttckStateBase  ()
    , m_ShotCoolDown	    (0.0f)
	, m_CoolTime		    (1.0f)
{
}

AttackLong::~AttackLong()
{
}

void AttackLong::Enter(Player* player)
{
	m_ShotCoolDown = m_CoolTime;
}

#if 0
//void AttackLong::ExecuteAttack(Player* player)
//{
//	constexpr float zero = 0.0f;
//	float deltaTime = Timer::GetInstance().DeltaTime();
//	PlayerContext ctx(player);
//
//	//クールタイムの処理.
//	if (m_CoolTime >= zero)
//	{
//		m_ShotCoolDown -= deltaTime;
//	}
//
//	////弾を撃つ動作.
//	//if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && m_ShotCoolDown <= zero)
//	//{      
// //       //弾を撃つときなんだけどWard座標とオフセットを足しているから横にずれると弾の生成される位置がずれる.
//
// //       D3DXMATRIX matS, matR, matT, playerWorldMatrix;
// //       //サイズ.
// //       D3DXMatrixScaling(&matS, ctx.Scale.x, ctx.Scale.y, ctx.Scale.z);
// //       //向き.
// //       //むきが入っていないので弾の出現位置が変更される.
// //       D3DXMatrixRotationYawPitchRoll(&matR, ctx.Rotation.y, ctx.Rotation.x, ctx.Rotation.z);
// //       //位置.
// //       D3DXMatrixTranslation(&matT, ctx.Position.x, ctx.Position.y, ctx.Position.z);
//
// //       D3DXMatrixMultiply(&playerWorldMatrix, &matS, &matR);
// //       D3DXMatrixMultiply(&playerWorldMatrix, &playerWorldMatrix, &matT);
//
// //       D3DXVECTOR3 worldBonePos;
// //       D3DXVec3TransformCoord(&worldBonePos, &ctx.BonePos, &playerWorldMatrix);
//
// //       // 弾の発射位置を計算(ワールドボーン位置 + オフセット)
// //       D3DXVECTOR3 shotPos = worldBonePos + m_ShotOffset;
//
// //       // 弾の発射方向を計算
// //       D3DXVECTOR3 Dir = player->Player_WS(ctx.Rotation.y);
// //       D3DXVec3Normalize(&Dir, &Dir);
//
// //       // Managerに弾を追加
// //       PShotManager::GetInstance()->AddPlayerShot(shotPos, Dir);
//
// //       // クールタイムリセット
// //       m_ShotCoolDown = m_CoolTime;
//
// //   }
//
//	static float Angle_Radion = 180.0f;
//	const float M_PI = 3.1415927f;
//	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
//	{
//		if (player->GetAttachMesh().expired()) return;
//		auto StaticMesh = std::dynamic_pointer_cast<SkinMesh>(player->GetAttachMesh().lock());
//		//ボーンの名前.
//		//blade_r_head
//		if (!StaticMesh) return;
//		const std::string TargetBoneName = "blade_r_head";
//		D3DXVECTOR3 Bone_World_Pos;
//
//		if (StaticMesh->GetPosFromBone(TargetBoneName.c_str(), &Bone_World_Pos));
//		{
//
//		}
//	}
//}
#endif

void AttackLong::ExecuteAttack(Player* player)
{
    //クールタイムのデルタタイムを取得.
    float deltaTime = Timer::GetInstance().DeltaTime();

    //クールタイム管理.
    m_ShotCoolDown -= deltaTime;
    if (m_ShotCoolDown > 0.0f) return;

    //SkinMeshの取得.
    if (player->GetAttachMesh().expired()) return;
    //dynamic_pointer_castを使用してSkinMeshClassの関数を取得する.
    auto Mesh = std::dynamic_pointer_cast<SkinMesh>(player->GetAttachMesh().lock());
    if (!Mesh) return;

    //発射元ボーン.
    //右の剣.
    //std::stringを使用してコードを書き直す.
    std::string BoneName = "blade_r_head";

    //ボーンのワールド位置.
    D3DXVECTOR3 BoneWorldPos;
    if (!Mesh->GetPosFromBone(BoneName.c_str(), &BoneWorldPos))
    {
        return;
    }

    //Playerのワールド行列.
    D3DXMATRIX MatS, MatR, MatT, MatWorld;

    //PlayerのPositionを取得.
    const D3DXVECTOR3& pos = player->GetPosition();
    //PlayerのRotationを取得.
    const D3DXVECTOR3& rot = player->GetRotation();
    //PlayerのScaleを取得.
    const D3DXVECTOR3& scl = player->GetScale();

    //Scale.
    D3DXMatrixScaling(&MatS, scl.x, scl.y, scl.z);
    //Rotation.
    D3DXMatrixRotationYawPitchRoll(&MatR, rot.y, rot.x, rot.z);
    //Position.
    D3DXMatrixTranslation(&MatT, pos.x, pos.y, pos.z);
    MatWorld = MatS * MatR * MatT;

    //ボーンのローカルオフセットを回転させる  
    D3DXVECTOR3 FinalShotPos = BoneWorldPos;

    //弾の向き.
    D3DXVECTOR3 Dir = player->Player_WS(rot.y);
    //正規化.
    D3DXVec3Normalize(&Dir, &Dir);

    //弾生成
    PShotManager::GetInstance()->AddPlayerShot(FinalShotPos, Dir);
    //クールタイム
    m_ShotCoolDown = m_CoolTime;
}

void AttackLong::Exit(Player* player)
{
}
