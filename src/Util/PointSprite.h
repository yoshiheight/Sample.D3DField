/**
 * @file
 * ポイントスプライト管理の宣言
 */
#ifndef __POINTSPRITE_H__
#define __POINTSPRITE_H__

/**
 * ポイントスプライト管理クラス
 */
class PointSprite
{
    NO_COPY_CONSTRUCT(PointSprite);
    NO_OPERATOR_EQUAL(PointSprite);

private:
    // コンストラクタ
    PointSprite();

public:
    // デストラクタ
    virtual ~PointSprite();

    // シングルトンの初期化
    static void NewInstance() { ASSERT( s_pInstance == NULL); s_pInstance = new PointSprite(); }

    // シングルトンの開放
    static void DeleteInstance() { delete s_pInstance; }

    // シングルトンインスタンスの取得
    static PointSprite* GetInstance() { return s_pInstance; }

    // ポイントスプライト描画
    void DrawSprite(LPDIRECT3DDEVICE9 pD3DDevice, const LPD3DXVECTOR3 pVector, int count);

    // バッファリングデータの描画
    void Flush(LPDIRECT3DDEVICE9 pD3DDevice);

private:
    // 頂点バッファの構造体
    struct POINTSPRITE_VERTEX
    {
        D3DXVECTOR3 vector; ///< 座標
        DWORD       color;  ///< 色
    };

    static const DWORD POINTSPRITE_FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

    // バッファリング
    void Buffering(const LPD3DXVECTOR3 pVector, int start, int count);

private:
    static PointSprite* s_pInstance;    

    static const INT32 BUFFER_MAX = 100;

    INT32               m_bufferCount;

    POINTSPRITE_VERTEX  m_Vertexs[BUFFER_MAX];
};

#endif // __GRAPHICS_H__
