/**
 * @file
 * テクスチャ管理の宣言
 */
#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include <string>
#include <map>

/**
 * テクスチャ管理クラス
 *
 * Direct3Dのテクスチャの参照カウント付キャッシュ機能を提供
 * @attention CreateTexture()で作成したテクスチャはReleaseTexture()で開放する必要あり
 */
class TextureManager
{
    NO_COPY_CONSTRUCT(TextureManager);
    NO_OPERATOR_EQUAL(TextureManager);

public:
    /**
     * テクスチャデータの構造体
     *
     * Direct3Dテクスチャと画像情報を保持
     */
    struct TEXTURE_DATA
    {
        std::wstring        fileName;       ///< テクスチャ画像ファイル名
        LPDIRECT3DTEXTURE9  pD3DTexture;    ///< Direct3Dテクスチャ
        D3DXIMAGE_INFO      d3dImageInfo;   ///< Direct3Dテクスチャ画像情報
    };

private:
    // コンストラクタ
    TextureManager();

public:
    // デストラクタ
    virtual ~TextureManager();

    // テクスチャーを生成
    TEXTURE_DATA* CreateTexture(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszTextureFileName);

    // テクスチャーを開放
    void ReleaseTexture(TEXTURE_DATA* pTextureData);

    // シングルトンの初期化
    static void NewInstance() { ASSERT(s_pInstance == NULL); s_pInstance = new TextureManager(); }

    // シングルトンの開放
    static void DeleteInstance() { delete s_pInstance; }

    // シングルトンインスタンスの取得
    static TextureManager*  GetInstance() { return s_pInstance; }

private:
    /**
     * テクスチャのキャッシュ情報の構造体
     *
     * Direct3Dテクスチャポインタと参照カウントを保持
     */
    struct SHARE_TEXTURE
    {
        TEXTURE_DATA    textureData;    ///< テクスチャデータ
        INT32           count;          ///< 参照カウント
    };

private:
    static TextureManager* s_pInstance;

    /// テクスチャーのキャッシュコンテナ
    std::map<std::wstring, SHARE_TEXTURE*> m_CacheMapTexture;
};

#endif // __TEXTUREMANAGER_H__
