/**
 * @file
 * 座標軸の定義
 */
#include "stdafx.h"
#include "CoordAxis.h"

/**
 * コンストラクタ
 */
CoordAxis::CoordAxis()
    : m_pD3DVBOrgLine(NULL)
{
}

/**
 * デストラクタ
 */
CoordAxis::~CoordAxis()
{
    Release();
}

/** 
 * 座標軸を生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL CoordAxis::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
    Release();

    if (pD3DDevice == NULL) {
        return FALSE;
    }

    // -------------------------------------------
    // 座標軸用
    // -------------------------------------------

    // オブジェクトのVertexBufferを生成
    if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(CUSTOMVERTEX_LINE) * 6,
                                        D3DUSAGE_WRITEONLY,
                                        CUSTOMVERTEX_FVF_LINE,
                                        D3DPOOL_MANAGED, &m_pD3DVBOrgLine, NULL)))
    {
        return FALSE;
    }

    // VertexBufferの中身を埋める
    CUSTOMVERTEX_LINE* pVertex;
    m_pD3DVBOrgLine->Lock(0, 0, (void**)&pVertex, 0);

    // 各頂点の位置
    float length = 1000.0f;
    pVertex[0].coord = D3DXVECTOR3(-length, 0.0f,       0.0f);      // X軸
    pVertex[1].coord = D3DXVECTOR3(length,  0.0f,       0.0f);
    pVertex[2].coord = D3DXVECTOR3(0.0f,    -length,    0.0f);      // Y軸
    pVertex[3].coord = D3DXVECTOR3(0.0f,    length,     0.0f);
    pVertex[4].coord = D3DXVECTOR3(0.0f,    0.0f,       -length);   // Z軸
    pVertex[5].coord = D3DXVECTOR3(0.0f,    0.0f,       length);

    // 各頂点の色
    pVertex[0].color = D3DXCOLOR(1.0f,  0.0f,   0.0f,   1.0f);  // X軸
    pVertex[1].color = D3DXCOLOR(1.0f,  0.0f,   0.0f,   1.0f);
    pVertex[2].color = D3DXCOLOR(0.0f,  1.0f,   0.0f,   1.0f);  // Y軸
    pVertex[3].color = D3DXCOLOR(0.0f,  1.0f,   0.0f,   1.0f);
    pVertex[4].color = D3DXCOLOR(0.5f,  0.5f,   0.5f,   1.0f);  // Z軸
    pVertex[5].color = D3DXCOLOR(0.5f,  0.5f,   0.5f,   1.0f);

    m_pD3DVBOrgLine->Unlock();

    return TRUE;
}

/**
 * 座標軸の破棄
 */
void CoordAxis::Release()
{
    // 座標軸用
    SAFE_RELEASE(m_pD3DVBOrgLine);// 頂点バッファの解放
}

/** 
 * 座標軸を描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void CoordAxis::Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps)
{
    // ライトオフ
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    // ワールド行列
    D3DXMATRIX mWorld;
    D3DXMatrixIdentity(&mWorld);
    pD3DDevice->SetTransform(D3DTS_WORLD, &mWorld); // 単位行列を設定

    // 頂点バッファの設定
    pD3DDevice->SetStreamSource(0, m_pD3DVBOrgLine, 0, sizeof(CUSTOMVERTEX_LINE));

    // 頂点バッファのフォーマットの設定
    pD3DDevice->SetFVF(CUSTOMVERTEX_FVF_LINE);

    pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);    // ラインリスト
}
