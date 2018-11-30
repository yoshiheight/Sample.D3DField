/**
 * @file
 * ライト管理の定義
 */
#include "StdAfx.h"
#include "Light.h"

// スピード定数
const float Light::MOVE_SPEED           = 10.0f;
const float Light::LIGHT_COLOR_SPEED    = 0.1f;

/**
 * コンストラクタ
 */
Light::Light()
    :   m_colorR(1.0f),         // ライト色R
        m_colorG(1.0f),         // ライト色G
        m_colorB(1.0f),         // ライト色B
        m_directionX(20.0f),    // ライト方向X
        m_directionY(-50.0f),   // ライト方向Y
        m_directionZ(0.0f),     // ライト方向Z
        m_isF4(FALSE),
        m_isLight(TRUE)
{
}

/**
 * デストラクタ
 */
Light::~Light()
{
}

std::wstring Light::GetStatus()
{
    TCHAR szFPS[1000];
    SPRINTF(szFPS, 1000,
            _T("光源色 ： R=%f, G=%f, B=%f\n")
            _T("光源方向 ： X=%f, Y=%f, Z=%f\n"),
            m_colorR, m_colorG, m_colorB,
            m_directionX, m_directionY, m_directionZ);
    return szFPS;
}

/**
 * キーアサイン
 */
void Light::Update(float timeSpan)
{
    BOOL isCtrl = (::GetKeyState(VK_CONTROL) < 0);

    if (::GetKeyState('1') < 0)
    {
        if (m_colorR < 1.0f) m_colorR += timeSpan * LIGHT_COLOR_SPEED;
    }
    else if (::GetKeyState('Q') < 0)
    {
        if (m_colorR > 0.01f) m_colorR -= timeSpan * LIGHT_COLOR_SPEED;
    }
    else if (::GetKeyState('2') < 0)
    {
        if (m_colorG < 1.0f) m_colorG += timeSpan * LIGHT_COLOR_SPEED;
    }
    else if (::GetKeyState('W') < 0)
    {
        if (m_colorG > 0.01f) m_colorG -= timeSpan * LIGHT_COLOR_SPEED;
    }
    else if (::GetKeyState('3') < 0)
    {
        if (m_colorB < 1.0f) m_colorB += timeSpan * LIGHT_COLOR_SPEED;
    }
    else if (::GetKeyState('E') < 0)
    {
        if (m_colorB > 0.01f) m_colorB -= timeSpan * LIGHT_COLOR_SPEED;
    }
    else if (::GetKeyState('A') < 0)
    {
        m_directionX += timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState('Z') < 0)
    {
        m_directionX -= timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState('S') < 0)
    {
        m_directionY += timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState('X') < 0)
    {
        m_directionY -= timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState('D') < 0)
    {
        m_directionZ += timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState('C') < 0)
    {
        m_directionZ -= timeSpan * MOVE_SPEED;
    }

    // ライト使用可否切り替え
    if (::GetKeyState(VK_F4) < 0)
    {
        // 初回のF4押下の場合
        if (!m_isF4)
        {
            m_isLight = !m_isLight;
        }
        m_isF4 = TRUE;
    }
    else
    {
        m_isF4 = FALSE;
    }
}

/**
 * ライトを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Light::Lighting(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // 光源ラインの描画
    if (m_isLight)
    {
        // 光源ライン保持
        m_lightLine[0].vector   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_lightLine[0].color    = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
        m_lightLine[1].vector   = D3DXVECTOR3(-m_directionX, -m_directionY, -m_directionZ);
        m_lightLine[1].color    = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

        pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

        // 頂点のフォーマットの設定
        pD3DDevice->SetFVF(CUSTOMVERTEX_FVF_LINE);

        // 光源ラインの描画
        pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
                                    1,
                                    m_lightLine, sizeof(m_lightLine[0]));
    }

    // 光源の設定
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, m_isLight); // ライティングモード

    if (m_isLight)
    {
        // ライト
        ::ZeroMemory(&m_D3DLight, sizeof(m_D3DLight));

        m_D3DLight.Type = D3DLIGHT_DIRECTIONAL; // ディレクショナルライト（平行光源）

        m_D3DLight.Diffuse.r = m_colorR;
        m_D3DLight.Diffuse.g = m_colorG;
        m_D3DLight.Diffuse.b = m_colorB;

        D3DXVECTOR3 vLight(m_directionX, m_directionY, m_directionZ);
        ::D3DXVec3Normalize(static_cast<D3DXVECTOR3*>(&(m_D3DLight.Direction)), &vLight);

        pD3DDevice->LightEnable(0, TRUE);
        pD3DDevice->SetLight(0, &m_D3DLight);
    }
}
