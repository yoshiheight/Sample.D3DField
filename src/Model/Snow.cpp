/**
 * @file
 * 雪管理の定義
 */
#include "StdAfx.h"
#include "Snow.h"
//#include "util/PointSprite.h"

/*
// Ｚソート用
static int CompareVector(const void* p1, const void* p2)
{
    const D3DXVECTOR3* pV1 = static_cast<const D3DXVECTOR3*>(p1);
    const D3DXVECTOR3* pV2 = static_cast<const D3DXVECTOR3*>(p2);

    if (pV1->z < pV2->z)
    {
        return 1;
    }
    else if (pV2->z < pV1->z)
    {
        return -1;
    }
    return 0;
}
*/

/**
 * コンストラクタ
 */
Snow::Snow() : m_pSnowTexture(NULL)
{
    // ポイントスプライト
    for (int i = 0; i < SNOW_MAX; i++)
    {
        float x = (::rand() % 800) / 10.0f - 40.0f;
        float y = (::rand() % 400) / 10.0f;
        float z = (::rand() % 800) / 10.0f - 40.0f;

        // 頂点
//      m_vSnow[i] = D3DXVECTOR3(x, y, z);
        m_SnowVertexs[i].vector = D3DXVECTOR3(x, y, z);
        m_SnowVertexs[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

/**
 * デストラクタ
 */
Snow::~Snow()
{
    Release();
}

/** 
 * 雪を生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL Snow::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
    Release();

    // テクスチャの読み込み
    //m_pSnowTexture = TextureManager::GetInstance()->CreateTexture(pD3DDevice, _T("resource/snow.png"));
    //m_pSnowTexture = TextureManager::GetInstance()->CreateTexture(pD3DDevice, _T("resource/snow.bmp"));
    m_pSnowTexture = TextureManager::GetInstance()->CreateTexture(pD3DDevice, _T("resource/snow.tga"));

    return TRUE;
}

/**
 * 雪の破棄
 */
void Snow::Release()
{
    TextureManager::GetInstance()->ReleaseTexture(m_pSnowTexture);
    m_pSnowTexture = NULL;
}

/**
 * 更新
 */
void Snow::Update(float timeSpan, const LPD3DXMATRIX pmView, const D3DXVECTOR3* pvViewPos)
{
    /*
    for (int i = 0; i < SNOW_MAX; i++)
    {
        m_vSnow[i].y -= timeSpan * 1.5f;

        if (m_vSnow[i].y < 0.0f)
        {
            m_vSnow[i].y += 40.0f;
        }

        float length = m_vSnow[i].z - pvViewPos->z;
        if (0.0f <= length)
        {
            if (40.0f < length)
            {
                m_vSnow[i].z -= 80.0f;
            }
        }
        else
        {
            if (length < -40.0f)
            {
                m_vSnow[i].z += 80.0f;
            }
        }

        length = m_vSnow[i].x - pvViewPos->x;
        if (0.0f <= length)
        {
            if (40.0f < length)
            {
                m_vSnow[i].x -= 80.0f;
            }
        }
        else
        {
            if (length < -40.0f)
            {
                m_vSnow[i].x += 80.0f;
            }
        }

        //// ビュー行列変換
        //::D3DXVec3TransformCoord(&m_vSnowSorted[i],
        //                        &m_vSnow[i],
        //                        pmView);
    }

    // Ｚソート
    //::qsort(m_vSnowSorted, SNOW_MAX, sizeof(m_vSnowSorted[0]), ::CompareVector);
    */

    for (int i = 0; i < SNOW_MAX; i++)
    {
        m_SnowVertexs[i].vector.y -= timeSpan * 1.5f;

        if (m_SnowVertexs[i].vector.y < 0.0f)
        {
            m_SnowVertexs[i].vector.y += 40.0f;
        }

        float length = m_SnowVertexs[i].vector.z - pvViewPos->z;
        if (0.0f <= length)
        {
            if (40.0f < length)
            {
                m_SnowVertexs[i].vector.z -= 80.0f;
            }
        }
        else if (length < -40.0f)
        {
            m_SnowVertexs[i].vector.z += 80.0f;
        }

        length = m_SnowVertexs[i].vector.x - pvViewPos->x;
        if (0.0f <= length)
        {
            if (40.0f < length)
            {
                m_SnowVertexs[i].vector.x -= 80.0f;
            }
        }
        else if (length < -40.0f)
        {
            m_SnowVertexs[i].vector.x += 80.0f;
        }
    }
}

/** 
 * 雪の描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Snow::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // テクスチャステージの設定
    /*
    // カラー成分の設定（テクスチャARG1の色情報をそのまま出力）
    pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    // アルファ成分の設定（テクスチャARG1のアルファ値をそのまま出力）
    //pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
    */
    pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    // アルファテスト
    pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x20);
    pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    // ブレンディング設定
    pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    // ポイントスプライト設定
    pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);          // ポイントスプライトを有効
    pD3DDevice->SetRenderState(D3DRS_POINTSIZE, ToDWordBit(0.3f));      // サイズ
    pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);           // スケーリングを有効
//  pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, ToDWordBit(64.0f)); // 最大サイズ
    pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, ToDWordBit(0.0f));   // スケール係数
    pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, ToDWordBit(0.0f));
    pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, ToDWordBit(1.0f));

    // 行列設定
/*
    D3DXMATRIX mOldView;
    pD3DDevice->GetTransform(D3DTS_VIEW, &mOldView);
*/
    D3DXMATRIX mMatrix;
    D3DXMatrixIdentity(&mMatrix); // 単位行列
    pD3DDevice->SetTransform(D3DTS_WORLD, &mMatrix);
//  pD3DDevice->SetTransform(D3DTS_VIEW, &mMatrix);

    // テクスチャー設定
    pD3DDevice->SetTexture(0, m_pSnowTexture->pD3DTexture);

    /*
    // 描画
    PointSprite::GetInstance()->DrawSprite(pD3DDevice, m_vSnow, SNOW_MAX);
    PointSprite::GetInstance()->Flush(pD3DDevice);
    */
    // ポイントスプライトの描画
    pD3DDevice->SetFVF(POINTSPRITE_FVF);
    pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
                                SNOW_MAX,
                                m_SnowVertexs, sizeof(m_SnowVertexs[0]));

    // 元に戻す
    pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
    pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pD3DDevice->SetTexture(0, NULL);
//  pD3DDevice->SetTransform(D3DTS_VIEW, &mOldView);
}
