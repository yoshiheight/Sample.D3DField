/**
 * @file
 * シーン管理の定義
 */
#include "StdAfx.h"
#include "GameApp.h"
#include "Scene.h"

/**
 * コンストラクタ
 */
Scene::Scene()
    :   m_pD3DFont(NULL),
        m_isF8(FALSE),
        m_isFog(FALSE)
{
}

/**
 * デストラクタ
 */
Scene::~Scene()
{
    Release();
}

/** 
 * シーンを生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL Scene::Create(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszFileName)
{
    Release();

    if(pD3DDevice == NULL) {
        return FALSE;
    }

    m_Camera.Init(pD3DDevice);
    m_CoordAxis.Create(pD3DDevice);
    m_MapThumbnail.Create(pD3DDevice, pszFileName);
    m_Field.Create(pD3DDevice, pszFileName);
    m_Snow.Create(pD3DDevice);

    // フォントの生成
    if (FAILED(D3DXCreateFont(pD3DDevice,
                        16,                 // 高さ
                        0,                  // 平均文字幅
                        FW_NORMAL,          // 太さ
                        1,                  // ミップマップレベル
                        FALSE,              // 斜体 
                        SHIFTJIS_CHARSET,   // キャラクタ・セット
                        OUT_DEFAULT_PRECIS, // 出力精度
                        DEFAULT_QUALITY,    // 品質
                        DEFAULT_PITCH || FF_DONTCARE,   // ピッチ
                        _T("ＭＳ ゴシック"),    // フォント名
                        &m_pD3DFont)))
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * シーンの破棄
 */
void Scene::Release()
{
    m_MapThumbnail.Release();
    m_Field.Release();
    m_Snow.Release();

    // フォントの開放
    SAFE_RELEASE(m_pD3DFont);
}

void Scene::Update(float timeSpan)
{
    // フォグON/OFF
    if (::GetKeyState(VK_F8) < 0)
    {
        // 初回のF8押下の場合
        if (!m_isF8)
        {
            m_isFog = !m_isFog;
        }
        m_isF8 = TRUE;
    }
    else
    {
        m_isF8 = FALSE;
    }

    // 更新
    m_Camera.Update(timeSpan);
    m_Light.Update(timeSpan);
    m_Field.Update(timeSpan);

    // 雪更新
    D3DXMATRIX mView;
    D3DXVECTOR3 vPos;
    m_Camera.GetMatrix(&mView);
    m_Camera.GetPos(&vPos);
    m_Snow.Update(timeSpan, &mView, &vPos);
}

/** 
 * シーンを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Scene::Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps)
{
    // カメラ設定
    m_Camera.Transform(pD3DDevice);

    // ライト設定
    m_Light.Lighting(pD3DDevice);

    // 頂点フォグ
    float density = 0.01f;
    pD3DDevice->SetRenderState(D3DRS_FOGENABLE, m_isFog);
    pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0,50,50,50));
    pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
//  pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*) &density));
    float fogNear   = 0.1f;
    float fogFar    = 80.0f;
    pD3DDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*) &fogNear));
    pD3DDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*) &fogFar));

    // フィールド
    m_Field.Draw(pD3DDevice, pD3DCaps);

    // 雪
    m_Snow.Draw(pD3DDevice);

    pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

    // 座標軸
    m_CoordAxis.Draw(pD3DDevice, pD3DCaps);

    // 高さマップサムネイル
    m_MapThumbnail.Draw(pD3DDevice);

    // ステータス表示
    DrawStatus(pD3DDevice);
}

/** 
 * シーンを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Scene::DrawStatus(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // 固定FPSの場合
    if (GameApp::GetInstance()->GetFPS()->IsFixedFPS())
    {
        std::wstring status;

        TCHAR szFPS[100];
        SPRINTF(szFPS, 100,
                _T("----- ステータス -----\n")
                _T("FPS:%f\n"),
                GameApp::GetInstance()->GetFPS()->GetFPS());

        status += szFPS;
        status += m_Field.GetStatus();
        status += m_Camera.GetStatus();
        status += m_Light.GetStatus();

        RECT rect = {10, 10, 0, 0}; // 描画領域
        m_pD3DFont->DrawText(
                    NULL,                       // 描画に使うスプライト
                    status.c_str(),             // 文字列
                    -1,                         // 文字数(NULL終端まで表示)
                    &rect,                      // 描画領域
                    DT_LEFT | DT_NOCLIP,        // 左揃え＆クリップしない
                    D3DCOLOR_XRGB(0, 255, 255));// 文字色

        // キーアサイン表示
        rect.top += 250;
        m_pD3DFont->DrawText(
                    NULL,                   // 描画に使うスプライト
                    _T("----- キーアサイン -----\n")
                    _T("モデルX軸移動 ： ← or →\n")
                    _T("モデルY軸移動 ： ↑ or ↓\n")
                    _T("モデルZ軸移動 ： PGUp or PGDn\n")
                    _T("（回転:Ctrl併用）\n")
                    _T("\n")
                    _T("カメラX軸移動 ： 1 or 3\n")
                    _T("カメラY軸移動 ： 5 or 2\n")
                    _T("カメラZ軸移動 ： 9 or 6\n")
                    _T("（回転:Ctrl併用）\n")
                    _T("カメラズーム ： + or -\n")
                    _T("\n")
                    _T("光源色R ： 1 or Q\n")
                    _T("光源色G ： 2 or W\n")
                    _T("光源色B ： 3 or E\n")
                    _T("光源方向X ： A or Z\n")
                    _T("光源方向X ： S or X\n")
                    _T("光源方向X ： D or C\n")
                    _T("\n")
                    _T("高さ単位 ： O or L\n")
                    _T("\n")
                    _T("F1 ： 固定FPS切替\n")
                    _T("F2 ： 頂点バッファ⇔インデックスバッファ\n")
                    _T("F3 ： 原点注視⇔任意注視\n")
                    _T("F4 ： ライトON/OFF\n")
                    _T("F5 ： FillMode:Solid/Wireframe\n")
                    _T("F6 ： 法線表示ON/OFF\n")
                    _T("F7 ： テクスチャON/OFF\n")
                    _T("F8 ： フォグON/OFF\n"),
                    // 文字列
                    -1,                         // 文字数(NULL終端まで表示)
                    &rect,                      // 描画領域
                    DT_LEFT | DT_NOCLIP,        // 左揃え＆クリップしない
                    D3DCOLOR_XRGB(0, 255, 255));// 文字色
    }
    // 非固定FPSの場合
    else
    {
        // FPS表示
        TCHAR szFPS[100];
        SPRINTF(szFPS, 100, _T("FPS:%f\n"), GameApp::GetInstance()->GetFPS()->GetFPS());

        RECT rect = {10, 10, 0, 0}; // 描画領域
        m_pD3DFont->DrawText(
                    NULL,                       // 描画に使うスプライト
                    szFPS,                      // 文字列
                    -1,                         // 文字数
                    &rect,                      // 描画領域
                    DT_LEFT | DT_NOCLIP,        // 左揃え＆クリップしない
                    D3DCOLOR_XRGB(0, 255, 255));// 文字色
    }
}
