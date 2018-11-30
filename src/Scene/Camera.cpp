/**
 * @file
 * カメラ管理の定義
 */
#include "StdAfx.h"
#include "Camera.h"

// スピード定数
const float Camera::MOVE_SPEED      = 10.0f;
const float Camera::ROTATE_SPEED    = 30.0f;
const float Camera::ZOOM_SPEED      = 10.0f;

/**
 * コンストラクタ
 */
Camera::Camera() 
    :   m_aspect(1.0f),     // アスペクト比
        m_moveX(0.0f),      // カメラのX軸移動量
        m_moveY(5.0f),      // カメラのY軸移動量
        m_moveZ(-10.0f),    // カメラのZ軸移動量
        m_rotateX(0.0f),    // カメラのX軸回転角度
        m_rotateY(0.0f),    // カメラのY軸回転角度
        m_rotateZ(0.0f),    // カメラのZ軸回転角度
        m_zoom(45.0f),      // カメラのズーム値
        m_isF3(FALSE),
        m_isOrgLook(TRUE)
{
}

/**
 * デストラクタ
 */
Camera::~Camera()
{
}

/**
 * カメラを生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL Camera::Init(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // ビューポートを取得
    D3DVIEWPORT9 vp;
    if (FAILED(pD3DDevice->GetViewport(&vp))) {
        return FALSE;
    }

    // ビューポートのアクペクト比を取得
    m_aspect = (float) vp.Width / (float) vp.Height;

    return TRUE;
}

std::wstring Camera::GetStatus()
{
    TCHAR szFPS[1000];
    SPRINTF(szFPS, 1000,
            _T("注視モード ： %s\n")
            _T("カメラ位置 ： X=%f, Y=%f, Z=%f\n")
            _T("カメラ回転 ： X=%f, Y=%f, Z=%f\n")
            _T("カメラ画角 ： %f\n"),
            ((m_isOrgLook) ? _T("原点注視"):_T("任意注視")),
            m_moveX, m_moveY, m_moveZ,
            m_rotateX, m_rotateY, m_rotateZ,
            m_zoom);
    return szFPS;
}

/**
 * 更新
 */
void Camera::Update(float timeSpan)
{
    BOOL isCtrl = (::GetKeyState(VK_CONTROL) < 0);

    // カメラ移動、回転、ズーム
    if (::GetKeyState(VK_NUMPAD1) < 0)
    {
        if (isCtrl)
        {
            m_rotateX += timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveX -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD3) < 0)
    {
        if (isCtrl)
        {
            m_rotateX -= timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveX += timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD2) < 0)
    {
        if (isCtrl)
        {
            m_rotateY += timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveY -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD5) < 0)
    {
        if (isCtrl)
        {
            m_rotateY -= timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveY += timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD6) < 0)
    {
        if (isCtrl)
        {
            m_rotateZ += timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveZ -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD9) < 0)
    {
        if (isCtrl)
        {
            m_rotateZ -= timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveZ += timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_ADD) < 0)
    {
        m_zoom -= timeSpan * ZOOM_SPEED;
    }
    else if (::GetKeyState(VK_SUBTRACT) < 0)
    {
        m_zoom += timeSpan * ZOOM_SPEED;
    }

    // 注視点切り替え
    if (::GetKeyState(VK_F3) < 0)
    {
        // 初回のF3押下の場合
        if (!m_isF3)
        {
            m_isOrgLook = !m_isOrgLook;
        }
        m_isF3 = TRUE;
    }
    else
    {
        m_isF3 = FALSE;
    }
}

void Camera::GetMatrix(D3DXMATRIX* pMatrix)
{
    // 注視点
    D3DXVECTOR3 vLook(0.0f, 0.0f, 0.0f);

    // 原点注視ではない場合
    if (!m_isOrgLook)
    {
        // 回転
        D3DXMATRIX mRotateX;
        D3DXMatrixIdentity(&mRotateX);  // 単位行列（必要？必要ない！）
        ::D3DXMatrixRotationX(&mRotateX, D3DXToRadian(m_rotateX));

        D3DXMATRIX mRotateY;
        D3DXMatrixIdentity(&mRotateY);  // 単位行列（必要？）
        ::D3DXMatrixRotationY(&mRotateY, D3DXToRadian(m_rotateY));

        D3DXMATRIX mRotateZ;
        D3DXMatrixIdentity(&mRotateZ);  // 単位行列（必要？）
        ::D3DXMatrixRotationZ(&mRotateZ, D3DXToRadian(m_rotateZ));

        D3DXMATRIX  mRotate;
        D3DXMatrixIdentity(&mRotate);   // 単位行列

        mRotate *= mRotateX;
        mRotate *= mRotateY;
        mRotate *= mRotateZ;

        // 注視点を算出
        ::D3DXVec3TransformCoord(&vLook, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &mRotate);
        vLook.x += m_moveX;
        vLook.y += m_moveY;
        vLook.z += m_moveZ;
    }

    // LookAtLH
    ::D3DXMatrixIdentity(pMatrix);
    ::D3DXMatrixLookAtLH(pMatrix, &D3DXVECTOR3(m_moveX, m_moveY, m_moveZ),  // カメラ位置
                                  &vLook,   // 注視点
                                  &D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // カメラの上方向ベクトル
}

/** 
 * カメラを設定
 * @param pD3DDevice DirectX3Dデバイス
 */
void Camera::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // ビュー行列
    D3DXMATRIX mView;
    GetMatrix(&mView);

    pD3DDevice->SetTransform(D3DTS_VIEW, &mView);

    // 射影行列
    D3DXMATRIX mProj;
    D3DXMatrixIdentity(&mProj);
    D3DXMatrixPerspectiveFovLH(&mProj,
                            D3DXToRadian(m_zoom),   // 画角
                            m_aspect,               // アスペクト比
                            0.5f,                   // Nearプレーン
                            1000.0f);               // Farプレーン

    pD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}
