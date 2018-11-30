/**
 * @file
 * ハイトマップサムネイル管理の宣言
 */
#ifndef __MAPTHUMBNAIL_H__
#define __MAPTHUMBNAIL_H__

#include "util/TextureManager.h"

/**
 * ハイトマップサムネイルを管理するクラス
 */
class MapThumbnail
{
    NO_COPY_CONSTRUCT(MapThumbnail);
    NO_OPERATOR_EQUAL(MapThumbnail);

public:
    // コンストラクタ
    MapThumbnail();

    // デストラクタ
    virtual ~MapThumbnail();

    // ハイトマップサムネイルを生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszFileName);

    // ハイトマップサムネイルの破棄
    void Release();

    // ハイトマップサムネイルを描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice);

private:
    // 高さマップサンプル用（DrawPrimitiveUp用）頂点バッファの構造体
    struct CUSTOMVERTEX_MAP
    {
        D3DXVECTOR3 vector; ///< 座標
        float       rhw;    ///< RHW
        D3DXVECTOR2 tex;    ///< テクスチャ
    };

    static const DWORD  CUSTOMVERTEX_FVF_MAP = (D3DFVF_XYZRHW | D3DFVF_TEX1);

    CUSTOMVERTEX_MAP m_Vertex[4]; // 高さマップサンプル

    // テクスチャ用
    TextureManager::TEXTURE_DATA* m_pHeightMap; // 高さマップ
};

#endif // __MAPTHUMBNAIL_H__
