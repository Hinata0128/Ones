#include "PlayerContext.h"
#include "GameObject/SkinMeshObject/Character/Player/Player.h"
#include "SkinMesh/SkinMesh.h"

PlayerContext::PlayerContext(Player* player) 
    : Position      (player->m_vPosition)
    , Rotation      (player->m_vRotation)
    , Scale         (player->m_vScale)
    , AnimNo        (player->m_AnimNo)
    , AnimTime      (player->m_AnimTime)
    , AnimSpeed     (player->m_AnimSpeed)
    , Mesh          (player->m_pMesh.get())
    , AnimCtrl      (player->m_pAnimCtrl)
    , BonePos       (player->m_BonePos)
{
}