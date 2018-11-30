/**
 * @file
 * 座標軸管理の定義
 */
#ifndef __COORDAXIS_H__
#define __COORDAXIS_H__

/**
 * 座標軸を管理するクラス
 */
class CoordAxis
{
    NO_COPY_CONSTRUCT(CoordAxis);
    NO_OPERATOR_EQUAL(CoordAxis);

public:
    // コンストラクタ
    CoordAxis();

    // デストラクタ
    virtual ~CoordAxis();

    // 座標軸を生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice);

    // 座標軸の破棄
    void Release();

    // 座標軸を描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps);

private:
    // -----------------------------------------------
    // 座標軸用（DrawPrimitiveUp用）
    // -----------------------------------------------
    // 頂点バッファの構造体
    struct CUSTOMVERTEX_LINE
    {
        D3DXVECTOR3 coord;  ///< 座標
        DWORD       color;  ///< 色
    };

    static const DWORD  CUSTOMVERTEX_FVF_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

    // 座標軸描画用
    LPDIRECT3DVERTEXBUFFER9 m_pD3DVBOrgLine;    // 頂点バッファへのポインタ
};

#endif // __COORDAXIS_H__
