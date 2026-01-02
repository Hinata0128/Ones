#pragma once

#include "System/01_Renderer/Renderer.h"

/**************************************************
*	ゲームオブジェクトクラス.
**/
class GameObject
{
public:
	GameObject();
	//継承を行う場合は、デストラクタにvirtualを付けて仮想関数にすること.
	virtual ~GameObject();

	//virtual 型 関数名 = 0; 純粋仮想関数.
	//子クラスに処理をお任せするので、ここでは名前だけ宣言して定義は書かない.
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//座標設定関数.
	void SetPosition( float x, float y, float z ){
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	};
	//x,y,zをうたなくてもいい関数.
	void SetPosition(const D3DXVECTOR3& pos)
	{
		m_Position = pos;
	}

	//座標取得関数.
	const D3DXVECTOR3& GetPosition() const {
		return m_Position;
	}

	//回転設定関数.
	void SetRotation( float x, float y, float z ){
		m_Rotation.x = x;
		m_Rotation.y = y;
		m_Rotation.z = z;
	};
	//x,y,zをうたなくてもいい関数.
	void SetRotation(const D3DXVECTOR3& rot)
	{
		m_Rotation = rot;
	}

	void SetRotationY(float y_rot)
	{
		m_Rotation.y = y_rot;
	}

	//回転取得関数.
	const D3DXVECTOR3& GetRotation() const {
		return m_Rotation;
	}

	void SetScale(float xyz) {
		m_Scale = D3DXVECTOR3(xyz, xyz, xyz);
	}

	void SetScale(const D3DXVECTOR3& newScale)
	{
		m_Scale = newScale;
	}

	//拡縮取得関数.
	const D3DXVECTOR3& GetScale() const {
		return m_Scale;
	}

	void AddPosition(const D3DXVECTOR3& delta) {
		m_Position += delta;
	}

protected://protectedは子クラスのみアクセス可能.
	D3DXVECTOR3	m_Position;
	D3DXVECTOR3	m_Rotation;
	D3DXVECTOR3	m_Scale;
};