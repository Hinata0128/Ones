#include "Camera.h"
#include <cmath>

Camera::Camera()
    : m_angleX(0.0f)
    , m_angleY(0.0f)
    , m_sensitivity(0.0006f)
    , m_isActive(true)
{
    m_offset = D3DXVECTOR3(1.5f, 5.0f, -15.0f); // 初期状態はTPS（少し右寄り）
    D3DXMatrixIdentity(&m_viewMatrix);
}

void Camera::Update(float mouseDx, float mouseDy, const D3DXVECTOR3& targetPos) {
    // 左右の回転角だけを更新
    m_angleX += mouseDx * m_sensitivity;

    // 上下の回転角（m_angleY）は更新しない、または常に 0 に固定
    m_angleY = 0.0f; // これで常に真横からの視点に固定されます

    // 回転行列の作成（angleYを0にするので、実質的に左右回転のみになる）
    D3DXMATRIX matRot;
    D3DXMatrixRotationYawPitchRoll(&matRot, m_angleX, m_angleY, 0.0f);

    // 回転行列をオフセットベクトルに適用
    D3DXVECTOR4 transformedOffset;
    D3DXVec3Transform(&transformedOffset, &m_offset, &matRot);

    D3DXVECTOR3 cameraPos = targetPos + D3DXVECTOR3(transformedOffset.x, transformedOffset.y, transformedOffset.z);

    // 注視点
    D3DXVECTOR3 lookAt = targetPos;
    lookAt.y += 1.5f; // キャラクターの胸〜頭あたりを注視

    D3DXVECTOR3 up(0, 1, 0);
    D3DXMatrixLookAtLH(&m_viewMatrix, &cameraPos, &lookAt, &up);
}

D3DXVECTOR3 Camera::GetForward() const {
    // プレイヤーの移動用なので、水平方向（Y=0）の前方を返す
    return D3DXVECTOR3(sinf(m_angleX), 0.0f, cosf(m_angleX));
}

D3DXVECTOR3 Camera::GetRight() const {
    // 前方ベクトルと上方向ベクトルの外積から右方向を算出
    D3DXVECTOR3 forward = GetForward();
    D3DXVECTOR3 right;
    D3DXVECTOR3 up(0, 1, 0);
    D3DXVec3Cross(&right, &up, &forward);
    return right;
}