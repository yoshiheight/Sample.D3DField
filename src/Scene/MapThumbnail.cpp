/**
 * @file
 * ハイトマップサムネイル管理の定義
 */
#include "StdAfx.h"
#include "MapThumbnail.h"

/**
 * コンストラクタ
 */
MapThumbnail::MapThumbnail()
    :   m_pHeightMap(NULL)
{
}

/**
 * デストラクタ
 */
MapThumbnail::~MapThumbnail()
{
    Release();
}

/** 
 * ハイトマップサムネイルを生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL MapThumbnail::Create(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszFileName)
{
    Release();

    if(pD3DDevice == NULL) {
        return FALSE;
    }

    // テクスチャの読み込み
    if (pszFileName != NULL)
    {
        m_pHeightMap = TextureManager::GetInstance()->CreateTexture(pD3DDevice, pszFileName);
    }
    return TRUE;
}

/**
 * ハイトマップサムネイルの破棄
 */
void MapThumbnail::Release()
{
    TextureManager::GetInstance()->ReleaseTexture(m_pHeightMap);
    m_pHeightMap = NULL;
}

/** 
 * 高さマップの描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void MapThumbnail::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
    if (m_pHeightMap != NULL)
    {
        // テクスチャの設定
        pD3DDevice->SetTexture(0, m_pHeightMap->pD3DTexture);

        float   aspect  = (float) m_pHeightMap->d3dImageInfo.Width / (float) m_pHeightMap->d3dImageInfo.Height;
        float   bottom  = 950.0f;
        float   height  = 200.0f;
        float   width   = height * aspect;

        m_Vertex[0].vector  = D3DXVECTOR3(0.0f, bottom - height, 0.0f);
        m_Vertex[0].rhw     = 1.0f;
        m_Vertex[0].tex     = D3DXVECTOR2(0.0f, 0.0f);

        m_Vertex[1].vector  = D3DXVECTOR3(width, bottom - height, 0.0f);
        m_Vertex[1].rhw     = 1.0f;
        m_Vertex[1].tex     = D3DXVECTOR2(1.0f, 0.0f);

        m_Vertex[2].vector  = D3DXVECTOR3(width, bottom, 0.0f);
        m_Vertex[2].rhw     = 1.0f;
        m_Vertex[2].tex     = D3DXVECTOR2(1.0f, 1.0f);

        m_Vertex[3].vector  = D3DXVECTOR3(0.0f, bottom, 0.0f);
        m_Vertex[3].rhw     = 1.0f;
        m_Vertex[3].tex     = D3DXVECTOR2(0.0f, 1.0f);

        // 頂点のフォーマットの設定
        pD3DDevice->SetFVF(CUSTOMVERTEX_FVF_MAP);

        // 描画
        pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,
                                    2,
                                    m_Vertex, sizeof(m_Vertex[0]));

        // 元に戻す
        pD3DDevice->SetTexture(0, NULL);
    }
}
