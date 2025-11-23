#include "CollisionManager.h"

#include "Effect/Effect.h"

#include <algorithm>

#include "System//02_Singleton//Timer//Timer.h"



CollisionManager::CollisionManager()
    : m_pPlayer(nullptr)
{
}

CollisionManager::~CollisionManager()
{
    Release();
}

void CollisionManager::Update()
{
    AllCollider(); 
}

void CollisionManager::Draw()
{
#ifdef _DEBUG
    //スフィア描画
    for (const auto& sphere : m_pBSphere)
    {
        if (sphere) sphere->Draw();	
    }

    //-----------------------------------------------------
    // 敵の弾の当たり判定描画
    //-----------------------------------------------------
    for (auto eShot : m_EnemyShots)
    {
        // 敵弾がアクティブで表示されているかを確認 (AllCollider のロジックと一致させる)
        if (eShot && eShot->IsDisplay() && eShot->IsActive())
        {
            // BoundingSphere を取得し、Draw() 関数を呼び出す
            eShot->GetBoundingSphere().Draw();
        }
    }

    //ボックス描画
    for (const auto& box : m_pBBox)
    {
        if (box) box->Draw();		
    }
#endif
}

//------------------------- Create -------------------------
void CollisionManager::Create()
{
    m_pBSphere.clear();	//スフィアクリア.
    m_pBBox.clear();	//ボックスクリア.
}

//------------------------- Release -------------------------
void CollisionManager::Release()
{
    m_pBSphere.clear();
    m_pBBox.clear();
    m_pPlayer.reset();
    m_vEnemies.clear();
    m_PlayerShots.clear();
    m_EnemyShots.clear();
}

void CollisionManager::AddSphere(const std::shared_ptr<BoundingSphere>& sphere)
{
    if (sphere) m_pBSphere.push_back(sphere); //スフィア登録.
}

void CollisionManager::AddBox(const std::shared_ptr<BoundingBox>& box)
{
    if (box) m_pBBox.push_back(box); //ボックス登録.
}

void CollisionManager::Clear()
{
}

void CollisionManager::AllCollider()
{
    if (!m_pPlayer) return;

    //プレイヤーの位置を同期
    m_pPlayer->GetBoundingSphere().SetPosition(m_pPlayer->GetHitCenter());

    for (auto enemy : m_vEnemies)
    {
        if (!enemy) continue;

        // 敵の当たり判定の中心を更新
        enemy->GetBoundingSphere().SetPosition(enemy->GetHitCenter());

        for (auto pShot : m_PlayerShots)
        {
            if (!pShot || !pShot->IsDisplay() || !pShot->IsActive()) continue;

            // プレイヤー弾の当たり判定の中心を更新
            pShot->GetBoundingSphere().SetPosition(pShot->GetPosition());

            if (pShot->GetBoundingSphere().IsHit(enemy->GetBoundingSphere()))
            {
                // プレイヤー弾の非表示化と移動
                pShot->SetDisplay(false);
                pShot->SetActive(false);
                pShot->SetPosition(D3DXVECTOR3(0.f, -100.f, 0.f));

                // 敵をリスポーン位置へ移動 (SetEnemyPosition -> SetPosition に修正)
                enemy->SetPosition(D3DXVECTOR3(0.f, 0.f, 20.f));

                // エフェクトの再生 (SetEnemyPosition -> GetPosition に修正)
                Effect::Play(Effect::Test0, enemy->GetPosition());

                // 衝突処理が完了したので、次の敵へ
                break;
            }
        }
    }

    for (auto eShot : m_EnemyShots)
    {
        if (!eShot || !eShot->IsDisplay() || !eShot->IsActive()) continue;

        eShot->GetBoundingSphere().SetPosition(eShot->GetPosition());

        if (eShot->GetBoundingSphere().IsHit(m_pPlayer->GetBoundingSphere()))
        {
            //出力に表示する.
            OutputDebugStringA("Enemy Shot hit Player!\n");
            m_pPlayer->Hit();

            if (m_pPlayer->GetHitPoint() <= 0.0f)
            {
                Effect::Play(Effect::Test0, eShot->GetPosition());
            }

            eShot->SetDisplay(false);
            eShot->SetActive(false);

            break;
        }
    }

    //----------------------------------------------
        // 近距離攻撃（剣） vs 敵
        //----------------------------------------------
    if (m_pShortAttackSphere)
    {
        // ① 剣の当たり判定位置を更新
        m_pShortAttackSphere->SetPosition(m_pPlayer->GetShortAttackCenter());

        const D3DXVECTOR3 kRespawnPos(0.f, 0.f, 20.f);

        for (auto enemy : m_vEnemies)
        {
            if (!enemy) continue;

            // ② リスポーン直後の敵は無視
            D3DXVECTOR3 diff = enemy->GetPosition() - kRespawnPos;
            float dist = D3DXVec3Length(&diff);
            if (dist < 0.1f) continue;

            // ③ 敵の当たり判定更新
            enemy->GetBoundingSphere().SetPosition(enemy->GetHitCenter());

            // ④ 剣の当たりと比較
            if (m_pShortAttackSphere->IsHit(enemy->GetBoundingSphere()))
            {
                // 敵リスポーン
                enemy->SetPosition(kRespawnPos);

                // エフェクト
                Effect::Play(Effect::Test0, enemy->GetPosition());

                OutputDebugStringA("Short Attack Hit Enemy!\n");
            }
        }
    }
}

bool CollisionManager::CheckSphereSphere(const BoundingSphere& a, const BoundingSphere& b)
{
    return a.IsHit(b);
}

bool CollisionManager::CheckBoxBox(const BoundingBox& a, const BoundingBox& b)
{
    return a.IsHit(b);
}

bool CollisionManager::CheckSphereBox(const BoundingSphere& sphere, const BoundingBox& box)
{
    // 球体とAABBの判定（後で拡張可能）.
    // 今は簡易版として中心距離で判定.
    D3DXVECTOR3 closest;
    closest.x = std::max<float>(box.GetMin().x, std::min<float>(sphere.GetPostion().x, box.GetMax().x));
    closest.y = std::max<float>(box.GetMin().y, std::min<float>(sphere.GetPostion().y, box.GetMax().y));
    closest.z = std::max<float>(box.GetMin().z, std::min<float>(sphere.GetPostion().z, box.GetMax().z));

    D3DXVECTOR3 diff = sphere.GetPostion() - closest;
    return D3DXVec3Length(&diff) <= sphere.GetRadius();
}
