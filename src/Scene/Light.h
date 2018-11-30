/**
 * @file
 * ライト管理の宣言
 */
#ifndef __LIGHT_H__
#define __LIGHT_H__

/**
 * ライトを管理するクラス
 */
class Light
{
    NO_COPY_CONSTRUCT(Light);
    NO_OPERATOR_EQUAL(Light);

public:
    // コンストラクタ
    Light();

    // デストラクタ
    virtual ~Light();

    // 更新
    void Update(float timeSpan);

    // ライト設定
    void Lighting(LPDIRECT3DDEVICE9 pD3DDevice);

    // ステータス文字列を取得
    std::wstring GetStatus();

private:
    // ライト方向描画用
    // 頂点バッファの構造体
    struct CUSTOMVERTEX_LINE
    {
        D3DXVECTOR3 vector; ///< 座標
        DWORD       color;  ///< 色
    };

    static const DWORD  CUSTOMVERTEX_FVF_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

    CUSTOMVERTEX_LINE       m_lightLine[2];     // 光源

    // 光源用
    D3DLIGHT9               m_D3DLight;         // ライト
    
    // ライト使用切り替え用
    BOOL                    m_isF4;
    BOOL                    m_isLight;          // ライト使用

    float       m_colorR;   // ライト色R
    float       m_colorG;   // ライト色G
    float       m_colorB;   // ライト色B

    float       m_directionX;   // ライト方向X
    float       m_directionY;   // ライト方向Y
    float       m_directionZ;   // ライト方向Z

    // スピード定数
    static const float  MOVE_SPEED;         // 光源移動
    static const float  LIGHT_COLOR_SPEED;  // 光源色
};

#endif // __LIGHT_H__
