/**
 * @file
 * シーン管理の宣言
 */
#ifndef __SCENE_H__
#define __SCENE_H__

#include "util/TextureManager.h"
#include "Camera.h"
#include "Light.h"
#include "CoordAxis.h"
#include "MapThumbnail.h"
#include "model/Field.h"
#include "model/Snow.h"

/**
 * シーンを管理するクラス
 */
class Scene
{
    NO_COPY_CONSTRUCT(Scene);
    NO_OPERATOR_EQUAL(Scene);

public:
    // コンストラクタ
    Scene();

    // デストラクタ
    virtual ~Scene();

    // シーンを生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszFileName);

    // キーアサイン
    void Update(float timeSpan);

    // シーンの破棄
    void Release();

    // シーンを描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps);

private:
    void DrawStatus(LPDIRECT3DDEVICE9 pD3DDevice);

private:
    Camera              m_Camera;   ///< カメラ
    Light               m_Light;
    CoordAxis           m_CoordAxis;
    MapThumbnail        m_MapThumbnail;
    Field               m_Field;

    Snow                m_Snow;

    // 文字列描画用
    LPD3DXFONT          m_pD3DFont;     // フォント

    // フォグ用
    BOOL                m_isF8;
    BOOL                m_isFog;
};

#endif // __SCENE_H__
