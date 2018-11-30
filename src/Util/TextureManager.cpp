/**
 * @file
 * テクスチャ管理の定義
 */
#include "StdAfx.h"
#include "TextureManager.h"

TextureManager* TextureManager::s_pInstance = NULL;

/**
 * コンストラクタ
 */
TextureManager::TextureManager()
{
}

/**
 * デストラクタ
 */
TextureManager::~TextureManager()
{
    ASSERT(m_CacheMapTexture.empty());
}

/**
 * テクスチャーを生成
 * @param pD3DDevice DirectX3Dデバイス
 * @param pszTextureFileName テクスチャファイル名
 * @return テクスチャーデータ
 */
TextureManager::TEXTURE_DATA* TextureManager::CreateTexture(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszTextureFileName)
{
    // キャッシュに既に存在する場合
    if (m_CacheMapTexture.count(pszTextureFileName))
    {
        SHARE_TEXTURE* pShare = m_CacheMapTexture[pszTextureFileName];

        // 参照カウントアップ
        pShare->count++;
        TRACE("Count Up Texture!!\n");

        return &pShare->textureData;
    }
    // 未読み込みの場合
    else
    {
        // テクスチャの読み込み
        LPDIRECT3DTEXTURE9 pD3DTexture = NULL;
        D3DXIMAGE_INFO d3dImageInfo;
        if (FAILED(::D3DXCreateTextureFromFileEx(pD3DDevice, pszTextureFileName,
                                                0, 0, 0, 0, D3DFMT_A8R8G8B8,// アルファ付きの 32 ビット
                                                                            // D3DFMT_X8R8G8B8  32 ビット
                                                                            // D3DFMT_R8G8B8    24 ビット
                                                D3DPOOL_MANAGED,
                                                D3DX_FILTER_LINEAR,
                                                D3DX_FILTER_LINEAR,
                                                D3DCOLOR_XRGB(0,0,0),
                                                &d3dImageInfo,
                                                NULL,
                                                &pD3DTexture)))
        {
            return NULL;
        }

        // キャッシュ要素を生成
        SHARE_TEXTURE* pShare = new SHARE_TEXTURE;
        pShare->count = 1;
        pShare->textureData.fileName        = pszTextureFileName;
        pShare->textureData.pD3DTexture     = pD3DTexture;
        pShare->textureData.d3dImageInfo    = d3dImageInfo;

        // キャッシュに追加
        m_CacheMapTexture[pszTextureFileName] = pShare;
        TRACE("Create Texture!!\n");

        return &pShare->textureData;
    }
}

/**
 * テクスチャーを開放
 * @param pTextureData テクスチャデータ
 */
void TextureManager::ReleaseTexture(TEXTURE_DATA* pTextureData)
{
    if (pTextureData != NULL && 0 < m_CacheMapTexture.count(pTextureData->fileName.c_str()))
    {
        SHARE_TEXTURE* pShare = m_CacheMapTexture[pTextureData->fileName.c_str()];

        // 参照カウントダウン
        pShare->count--;
        TRACE("Count Down Texture!!\n");

        // 参照カウントが0の場合
        if (pShare->count == 0)
        {
            // テクスチャ開放
            SAFE_RELEASE(pShare->textureData.pD3DTexture);

            // キャッシュ要素削除
            m_CacheMapTexture.erase(pShare->textureData.fileName.c_str());
            delete pShare;

            TRACE("Release Texture!!\n");
        }
    }
}
