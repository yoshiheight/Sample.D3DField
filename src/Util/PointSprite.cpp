/**
 * @file
 * ポイントスプライト管理の定義
 */
#include "StdAfx.h"
#include "PointSprite.h"

PointSprite* PointSprite::s_pInstance = NULL;

/**
 * コンストラクタ
 */
PointSprite::PointSprite()
    :   m_bufferCount(0)
{
}

/**
 * デストラクタ
 */
PointSprite::~PointSprite()
{
}

void PointSprite::Buffering(const LPD3DXVECTOR3 pVector, int start, int count)
{
    INT32 remain = count - start; // 残数
    INT32 min = min(remain, BUFFER_MAX);

    for (int i = 0; i < min; i++)
    {
        m_Vertexs[m_bufferCount].vector = pVector[start + i];
        m_Vertexs[m_bufferCount].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        m_bufferCount++;
    }
}

void PointSprite::DrawSprite(LPDIRECT3DDEVICE9 pD3DDevice, const LPD3DXVECTOR3 pVector, int count)
{
    // 頂点フォーマットの設定
    pD3DDevice->SetFVF(POINTSPRITE_FVF);

    INT32 start = 0;
    while (start < count)
    {
        // バッファリング
        Buffering(pVector, start, count);
        start += m_bufferCount;

        if (BUFFER_MAX <= m_bufferCount)
        {
            // ポイントスプライトの描画
            pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
                                    m_bufferCount,
                                    m_Vertexs, sizeof(m_Vertexs[0]));
            m_bufferCount = 0;
        }
    }
}

void PointSprite::Flush(LPDIRECT3DDEVICE9 pD3DDevice)
{
    if (0 < m_bufferCount)
    {
        // 頂点フォーマットの設定
        pD3DDevice->SetFVF(POINTSPRITE_FVF);

        // ポイントスプライトの描画
        pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
                                    m_bufferCount,
                                    m_Vertexs, sizeof(m_Vertexs[0]));

        m_bufferCount = 0;
    }
}
