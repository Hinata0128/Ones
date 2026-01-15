#pragma once
#include <d3dx10math.h>

class Camera {
public:
    // 参照を返すシングルトンインスタンス
    static Camera& GetInstance() {
        static Camera instance; // 初回呼び出し時に生成される（C++11以降スレッドセーフ）
        return instance;
    }

    // コピーと代入を禁止
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    void Update(float mouseDx, float mouseDy, const D3DXVECTOR3& targetPos);

    // 行列・ベクトル取得
    const D3DXMATRIX& GetViewMatrix() const { return m_viewMatrix; }
    D3DXVECTOR3 GetForward() const;
    D3DXVECTOR3 GetRight() const;

    // 設定変更（FPS/TPS切り替え用）
    void SetOffset(const D3DXVECTOR3& offset) { m_offset = offset; }

    void SetActive(bool active) { m_isActive = active; }
    bool IsActive() const { return m_isActive; }
private:
    Camera();
    ~Camera() {}

    D3DXMATRIX  m_viewMatrix;
    D3DXVECTOR3 m_offset;      // ターゲットからの相対距離
    float       m_angleX;      // ヨー（左右）
    float       m_angleY;      // ピッチ（上下）
    float       m_sensitivity;

    bool m_isActive;
};