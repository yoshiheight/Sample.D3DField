/**
 * @file
 * カメラ管理の宣言
 */
#ifndef __CAMERA_H__
#define __CAMERA_H__

/**
 * カメラを管理するクラス
 */
class Camera
{
    NO_COPY_CONSTRUCT(Camera);
    NO_OPERATOR_EQUAL(Camera);

public:
    // コンストラクタ
    Camera();

    // デストラクタ
    virtual ~Camera();

    // カメラを生成
    BOOL Init(LPDIRECT3DDEVICE9 pD3DDevice);

    // 更新
    void Update(float timeSpan);

    // 変換行列設定
    void Transform(LPDIRECT3DDEVICE9 pD3DDevice);

    void GetMatrix(D3DXMATRIX* pMatrix);

    void GetPos(D3DXVECTOR3* pvPos)
    {
        pvPos->x = m_moveX;
        pvPos->y = m_moveY;
        pvPos->z = m_moveZ;
    }

    // ステータス文字列を取得
    std::wstring GetStatus();

private:
    // アスペクト比
    float                   m_aspect;

    // 注視点切り替え用
    BOOL                    m_isF3;
    BOOL                    m_isOrgLook;    // 原点注視かどうか

    float       m_moveX;    // カメラのX軸移動量
    float       m_moveY;    // カメラのY軸移動量
    float       m_moveZ;    // カメラのZ軸移動量

    float       m_rotateX;  // カメラのX軸回転角度
    float       m_rotateY;  // カメラのY軸回転角度
    float       m_rotateZ;  // カメラのZ軸回転角度

    float       m_zoom;     // カメラのズーム値

    // スピード定数
    static const float  MOVE_SPEED;         // モデル、カメラ、光源移動
    static const float  ROTATE_SPEED;       // モデル、カメラ回転
    static const float  ZOOM_SPEED;         // カメラズーム
};

#endif // __CAMERA_H__
