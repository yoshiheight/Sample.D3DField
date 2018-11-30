/**
 * @file
 * 雪管理の宣言
 */
#ifndef __SNOW_H__
#define __SNOW_H__

#include "util/TextureManager.h"

/**
 * 雪を管理するクラス
 */
class Snow
{
    NO_COPY_CONSTRUCT(Snow);
    NO_OPERATOR_EQUAL(Snow);

public:
    // コンストラクタ
    Snow();

    // デストラクタ
    virtual ~Snow();

    // 生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice);

    // 更新
    void Update(float timeSpan, const LPD3DXMATRIX pmView, const D3DXVECTOR3* pvViewPos);

    // 破棄
    void Release();

    // 描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice);

private:
    // 頂点バッファの構造体
    struct POINTSPRITE_VERTEX
    {
        D3DXVECTOR3 vector; ///< 座標
        DWORD       color;  ///< 色
    };

    static const DWORD POINTSPRITE_FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

    static const INT32 SNOW_MAX = 10001;

    POINTSPRITE_VERTEX  m_SnowVertexs[SNOW_MAX]; ///< 雪頂点（ワールド座標系）
//  D3DXVECTOR3     m_vSnow[SNOW_MAX];          ///< 座標（ワールド座標系）
//  D3DXVECTOR3     m_vSnowSorted[SNOW_MAX];    ///< 座標（ビュー座標系）

    TextureManager::TEXTURE_DATA*   m_pSnowTexture;
};

#endif // __SNOW_H__
