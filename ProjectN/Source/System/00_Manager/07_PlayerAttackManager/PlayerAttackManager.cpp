#include "PlayerAttackManager.h"

#include "GameObject/SkinMeshObject/Character/Player/Player.h"

#include "GameObject/SkinMeshObject/Character/Player/PlayerAttckStateBase/AttackLong/AttackLong.h" 
#include "GameObject/SkinMeshObject/Character/Player/PlayerAttckStateBase/AttackShort/AttackShort.h"
#include "GameObject/SkinMeshObject/Character/Player/PlayerAttckStateBase/NoAttack/NoAttack.h"

// ---------------------------------------------------------------------

PlayerAttackManager::PlayerAttackManager(Player* pOwner)
    : ManagerBase()
    , m_pOwner(pOwner)
    , m_pCurrentAttackState(nullptr) 
{
    RegisterStates();
    // 最初のStateを設定し、Enter()を呼び出す
    ChangeAttackState(enAttack::Short);
}

PlayerAttackManager::~PlayerAttackManager()
{
}

// ---------------------------------------------------------------------

void PlayerAttackManager::Update()
{
    if (m_pCurrentAttackState && m_pOwner)
    {
        m_pCurrentAttackState->ExecuteAttack(m_pOwner);
    }
    ManagerBase::Update();
}

void PlayerAttackManager::Draw()
{
    // m_pCurrentAttackState は PlayerAttckStateBase* 型
    if (m_pCurrentAttackState && m_pOwner)
    {
        // 現在のステートが AttackShort であるかを確認し、ダウンキャスト
        if (auto pShort = dynamic_cast<AttackShort*>(m_pCurrentAttackState))
        {
            // AttackShort に固有の Draw 関数を呼び出す
            pShort->Draw(m_pOwner); // ★ 呼び出し場所
        }
    }
    // Baseクラスの処理を呼ぶ（あれば）
    ManagerBase::Draw();
}

void PlayerAttackManager::Create()
{
    ManagerBase::Create();
}

void PlayerAttackManager::Release()
{
    m_StateMap.clear();
    ManagerBase::Release();
}

void PlayerAttackManager::CleanUpState(enAttack en)
{
    m_StateMap[en]->Enter(m_pOwner);
}

// ---------------------------------------------------------------------

void PlayerAttackManager::ChangeAttackState(enAttack type)
{
    if (m_StateMap.count(type) == 0) return;

    PlayerAttckStateBase* nextState = m_StateMap.at(type).get();

    if (m_pCurrentAttackState != nextState)
    {
        if (m_pCurrentAttackState) {
            m_pCurrentAttackState->Exit(m_pOwner);
        }

        m_pCurrentAttackState = nextState;

        if (m_pCurrentAttackState) {
            m_pCurrentAttackState->Enter(m_pOwner);
        }
    }
}

AttackShort* PlayerAttackManager::GetCurrentShortAttack() const
{
    // マップに enAttack::Short が存在するか確認
    if (m_StateMap.count(enAttack::Short) == 0)
    {
        return nullptr;
    }

    // マップから該当する unique_ptr を取得し、その raw ポインタを取得
    PlayerAttckStateBase* baseState = m_StateMap.at(enAttack::Short).get();

    // 取得したポインタを AttackShort* にダウンキャストして返す
    // dynamic_cast を使用すると、安全に型チェックを行えます。
    return dynamic_cast<AttackShort*>(baseState);
}
// ---------------------------------------------------------------------

void PlayerAttackManager::RegisterStates()
{
    m_StateMap.emplace(enAttack::Long, std::make_unique<AttackLong>());
    m_StateMap.emplace(enAttack::Short, std::make_unique<AttackShort>());
    m_StateMap.emplace(enAttack::NoAttack, std::make_unique<NoAttack>());
}