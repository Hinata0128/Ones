#include "Shadow.h"

#include "DirectX/DirectX11.h"
#include "Sprite3D/Sprite3D.h"

Shadow::Shadow()
	: SpriteObject	()
	, m_pTarget		(nullptr)
	, m_ShadowSprite(std::make_shared<Sprite3D>())

{
	Create();
}

Shadow::~Shadow()
{
}

void Shadow::Update()
{
	if (m_pTarget == nullptr) return;

	// 1. 対象の座標を取得
	D3DXVECTOR3 targetPos = m_pTarget->GetPosition();

	// 2. 自分の座標（SpriteObjectのメンバ）を更新
	m_Position.x = targetPos.x;
	m_Position.z = targetPos.z;
	m_Position.y = 0.01f; // 地面埋まり防止のため少し高めに設定

	// 3. スケールの設定
	// プレイヤーのスケールが0.05と小さい場合、そのまま掛けると影も消えるので注意
	m_Scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);

	// 4. 重要：自分自身の設定をスプライトに反映させる
	// SpriteObject::Updateの中でこの処理がない場合、明示的に書く必要があります
	m_ShadowSprite->SetPosition(m_Position);
	m_ShadowSprite->SetRotation(m_Rotation);
	m_ShadowSprite->SetScale(m_Scale);

	SpriteObject::Update();
}

void Shadow::Draw()
{
	// 基底クラス(SpriteObject)のDrawを呼ぶ前に、最新の行列をセットする
	auto dx11 = DirectX11::GetInstance();

	DirectX11::GetInstance()->SetDepth(false);
	SpriteObject::Draw(); // これで内部のRenderに行列が渡る
	DirectX11::GetInstance()->SetDepth(true);

	dx11->SetAlphaBlend(false);
	dx11->SetDepth(true);
}

void Shadow::Create()
{
	Sprite3D::SPRITE_STATE SSShadow =
	{
		1.0f, 1.0f,
		128.0, 128.0,
		128.0, 128.0,
	};
	//画像の読み込み.
	m_ShadowSprite->Init(_T("Data\\Texture\\shadow.png"), SSShadow);

	this->AttachSprite(*m_ShadowSprite);

	//影を地面に寝かせる.
	m_Rotation.x = D3DXToRadian(90.0f);
}
