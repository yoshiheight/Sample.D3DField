/**
 * @file
 * フィールド管理の宣言
 */
#ifndef __FIELD_H__
#define __FIELD_H__

#include "util/TextureManager.h"

/**
 * フィールドを管理するクラス
 */
class Field
{
    NO_COPY_CONSTRUCT(Field);
    NO_OPERATOR_EQUAL(Field);

public:
    // コンストラクタ
    Field();

    // デストラクタ
    virtual ~Field();

public:
    // 生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszFileName);

    // 更新
    void Update(float timeSpan);

    // 破棄
    void Release();

    // 描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps);

    // ステータス文字列を取得
    std::wstring GetStatus() const;

private:
    // 頂点バッファを生成
    BOOL CreateVertexBuffer(LPDIRECT3DDEVICE9 pD3DDevice);

    // インデックスバッファを生成
    BOOL CreateIndexBuffer(LPDIRECT3DDEVICE9 pD3DDevice);

private:
    // 三角プリミティブ数を取得する
    inline INT32 GetPrimitiveCount() const { return m_meshCountCX * m_meshCountCY * 2; }

private:
    // 頂点バッファの構造体
    struct CUSTOMVERTEX
    {
        D3DXVECTOR3 vector; ///< 座標
        D3DXVECTOR3 normal; ///< 法線ベクトル
        DWORD       color;  ///< 色
        D3DXVECTOR2 tex;    ///< テクスチャ
    };

    // 頂点FVF
    static const DWORD  CUSTOMVERTEX_FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

    // 法線の描画用の頂点バッファの構造体
    struct CUSTOMVERTEX_LINE
    {
        D3DXVECTOR3 vector; ///< 座標
        DWORD       color;  ///< 色
    };

    // 頂点FVF（法線用）
    static const DWORD  CUSTOMVERTEX_FVF_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

private:
    // モデル定義情報
    CUSTOMVERTEX*           m_pvModel;

    // 頂点バッファでの描画用
    LPDIRECT3DVERTEXBUFFER9 m_pD3DVB;       ///< 頂点バッファ

    // インデックスバッファでの描画用
    LPDIRECT3DVERTEXBUFFER9 m_pD3DVB2;      ///< 頂点バッファ（インデックスバッファと併用）
    LPDIRECT3DINDEXBUFFER9  m_pD3DIdxB;     ///< インデックスバッファ

    CUSTOMVERTEX_LINE*      m_pNormalLine;  // 法線

    TextureManager::TEXTURE_DATA*   m_pTexture; // 地面用テクスチャ

    INT32           m_meshCountCX;      ///< 横のメッシュ数
    INT32           m_meshCountCY;      ///< 縦のメッシュ数
    D3DMATERIAL9    m_D3DMaterial;      ///< マテリアル
    float           m_heightUnit;       ///< 高さマップ１段階の高さ
    TextureManager::TEXTURE_DATA*   m_pHeightMap;   ///< 高さマップ

    // 移動量
    float       m_moveX;    ///< X軸移動量
    float       m_moveY;    ///< Y軸移動量
    float       m_moveZ;    ///< Z軸移動量

    // 回転角度
    float       m_rotateX;  ///< X軸回転角度
    float       m_rotateY;  ///< Y軸回転角度
    float       m_rotateZ;  ///< Z軸回転角度

    // 頂点バッファとインデックスバッファの切り替え用
    BOOL    m_isF2;         ///< F2キー初回押下フラグ
    BOOL    m_isDrawVB;     ///< 頂点バッファ描画フラグ

    // Fillモード切り替え用
    BOOL    m_isF5;         ///< F5キー初回押下フラグ
    BOOL    m_isSolid;      ///< ソリッド描画フラグ

    // 法線ライン表示の切り替え用
    BOOL    m_isF6;         ///< F6キー初回押下フラグ
    BOOL    m_isDrawNormal; ///< 法線ライン描画フラグ

    // テクスチャ表示の切り替え用
    BOOL    m_isF7;         ///< F7キー初回押下フラグ
    BOOL    m_isTexture;    ///< テクスチャー描画フラグ

    // 速度定数
    static const float  MOVE_SPEED;     ///< 移動速度
    static const float  ROTATE_SPEED;   ///< 回転速度
};

#endif // __FIELD_H__
