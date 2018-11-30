/**
 * @file
 * フィールド管理の定義
 */
#include "StdAfx.h"
#include "Field.h"

const float Field::MOVE_SPEED       = 10.0f;
const float Field::ROTATE_SPEED = 30.0f;

// 適当な色を返す
static D3DXCOLOR RandColor(/*INT32 index*/)
{
    // 色
    /*
    float r = ((index % 3) == 0) ? 1.0f : 0.0f;
    float g = ((index % 3) == 1) ? 1.0f : 0.0f;
    float b = ((index % 3) == 2) ? 1.0f : 0.0f;
    */
    INT32 rand = ::rand();
    float r = ((rand % 3) == 0) ? 1.0f : 0.0f;
    float g = ((rand % 3) == 1) ? 1.0f : 0.0f;
    float b = ((rand % 3) == 2) ? 1.0f : 0.0f;

    return D3DXCOLOR(r, g, b, 1.0f);
    //return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

/**
 * コンストラクタ
 */
Field::Field()
    :   m_meshCountCX(50),      // 横メッシュ数
        m_meshCountCY(50),      // 縦メッシュ数
        m_pTexture(NULL),       // 地面用テクスチャ
        m_pHeightMap(NULL),     // 高さマップ
        m_pvModel(NULL),        // モデル

        m_pNormalLine(NULL),    // 法線
        m_pD3DVB(NULL),         // 頂点バッファ用
        m_pD3DVB2(NULL),        // インデックスバッファ用
        m_pD3DIdxB(NULL),       // インデックスバッファ用
        m_moveX(0.0f),          // X軸移動量
        m_moveY(0.0f),          // Y軸移動量
        m_moveZ(0.0f),          // Z軸移動量
        m_rotateX(0.0f),        // X軸回転角度
        m_rotateY(0.0f),        // Y軸回転角度
        m_rotateZ(0.0f),        // Z軸回転角度

        m_isF6(FALSE),
        m_isDrawNormal(FALSE),
        m_heightUnit(0.1f),     // 高さマップ１段階の高さ
        m_isF7(FALSE),
        m_isDrawVB(TRUE),
        m_isF2(FALSE),
        m_isTexture(FALSE),
        m_isF5(FALSE),
        m_isSolid(TRUE)
{
    // マテリアル初期化
    ::ZeroMemory(&m_D3DMaterial, sizeof(m_D3DMaterial));
    m_D3DMaterial.Diffuse.r     = 1.0f;
    m_D3DMaterial.Diffuse.g     = 1.0f;
    m_D3DMaterial.Diffuse.b     = 1.0f;
    m_D3DMaterial.Diffuse.a     = 1.0f;
    m_D3DMaterial.Specular.r    = 1.0f;
    m_D3DMaterial.Specular.g    = 1.0f;
    m_D3DMaterial.Specular.b    = 1.0f;
    m_D3DMaterial.Specular.a    = 1.0f;
    m_D3DMaterial.Power         = 3.0f;

    // モデル情報初期化
    m_pvModel = new CUSTOMVERTEX[(m_meshCountCX + 1) * (m_meshCountCY + 1)];
    INT32 index = 0;
    for (INT32 z = 0; z < (m_meshCountCY + 1); z++)
    {
        for (INT32 x = 0; x < (m_meshCountCX + 1); x++)
        {
            float fX = x - (m_meshCountCX / 2.0f); // モデルの重心を原点とする
            float fZ = z - (m_meshCountCY / 2.0f); // モデルの重心を原点とする

            fZ *= -1;   // Z軸反転

//          float fY = (index % 10) / 10.0f;    // 高さを凸凹させる
//          float fY = (::rand() % 10) / ((x + 1) / 2.0f);  // 高さを凸凹させる
            float fY = (::rand() % 10) / 10.0f; // 高さを凸凹させる
//          float fY = 0.0f;

            m_pvModel[index].vector = D3DXVECTOR3(fX, fY, fZ);      // 頂点
            m_pvModel[index].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);// 法線
            m_pvModel[index].color  = ::RandColor(/*index*/);       // 色
            m_pvModel[index].tex    = D3DXVECTOR2(0.0f, 0.0f);      // テクスチャ

            index++;
        }
    }
}

/**
 * デストラクタ
 */
Field::~Field()
{
    // モデル定義の解放
    SAFE_DELETE(m_pvModel);

    Release();
}

/** 
 * フィールドを生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL Field::Create(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pszFileName)
{
    Release();

    // 高さマップ読み込み
    if (pszFileName != NULL)
    {
        // テクスチャの読み込み
        m_pHeightMap = TextureManager::GetInstance()->CreateTexture(pD3DDevice, pszFileName);

        INT32 vertexCnt = 0;
        D3DLOCKED_RECT lockedRect;
        if (SUCCEEDED(m_pHeightMap->pD3DTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_READONLY)))
        {
            m_meshCountCX = m_pHeightMap->d3dImageInfo.Width - 1;
            m_meshCountCY = m_pHeightMap->d3dImageInfo.Height - 1;
            m_pvModel = new CUSTOMVERTEX[(m_meshCountCX + 1) * (m_meshCountCY + 1)];

            for (UINT32 z = 0; z < m_pHeightMap->d3dImageInfo.Height; z++)
            {
                LPDWORD pBits = reinterpret_cast<LPDWORD>(static_cast<LPBYTE>(lockedRect.pBits) + (lockedRect.Pitch * z));

                for (UINT32 x = 0; x < m_pHeightMap->d3dImageInfo.Width; x++)
                {
                    float fX = static_cast<float>(x) - m_meshCountCX / 2.0f; // モデルの中心を原点とする為
                    float fZ = static_cast<float>(z) - m_meshCountCY / 2.0f; // モデルの中心を原点とする為

                    fZ *= -1;   // Z軸反転

                    float fTU   = x / static_cast<float>(m_meshCountCX);
                    float fTV   = z / static_cast<float>(m_meshCountCY);

//                  BYTE color = *(reinterpret_cast<LPBYTE>(pBits++));
                    DWORD color = *(pBits++);

                    BYTE r = LOBYTE(HIWORD(color));
                    BYTE g = HIBYTE(LOWORD(color));
                    BYTE b = LOBYTE(LOWORD(color));
                    float fY = (r + g + b) / 3.0f * m_heightUnit;

                    m_pvModel[vertexCnt].vector = D3DXVECTOR3(fX, fY, fZ);              // 頂点
                    m_pvModel[vertexCnt].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);        // 法線
                    m_pvModel[vertexCnt].color  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // 色
                    m_pvModel[vertexCnt].tex    = D3DXVECTOR2(fTU, fTV);                // テクスチャ

                    vertexCnt++;
                }
            }

            m_pHeightMap->pD3DTexture->UnlockRect(0);
        }
    }

    // 貼り付けテクスチャの読み込み
    m_pTexture = TextureManager::GetInstance()->CreateTexture(pD3DDevice, _T("resource/texture.bmp"));

    if (!CreateVertexBuffer(pD3DDevice))
    {
        return FALSE;
    }

    if (!CreateIndexBuffer(pD3DDevice))
    {
        return FALSE;
    }

    return TRUE;
}

// メッシュのモデル座標設定（頂点バッファで描画用）
BOOL Field::CreateVertexBuffer(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // 光源、法線の表示用
    m_pNormalLine = new CUSTOMVERTEX_LINE[GetPrimitiveCount() * 2];
    INT32 normalLineCount = 0;

    // オブジェクトのVertexBufferを生成
    if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(CUSTOMVERTEX) * GetPrimitiveCount() * 3,
                                            D3DUSAGE_WRITEONLY,
                                            CUSTOMVERTEX_FVF,
                                            D3DPOOL_MANAGED, &m_pD3DVB, NULL)))
    {
        return FALSE;
    }

    // VertexBufferの中身を埋める
    CUSTOMVERTEX* pVertex;
    m_pD3DVB->Lock(0, 0, (void**)&pVertex, 0);

    // メッシュループ
    INT32 vertexCnt = 0;
    for (INT32 z = 0; z < m_meshCountCY; z++)
    {
        for (INT32 x = 0; x < m_meshCountCX; x++)
        {
            INT32 index0 = z * (m_meshCountCX + 1) + x;
            INT32 index1 = (z + 1) * (m_meshCountCX + 1) + x + 1;
            INT32 index2 = (z + 1) * (m_meshCountCX + 1) + x;
            INT32 index3 = z * (m_meshCountCX + 1) + x;
            INT32 index4 = z * (m_meshCountCX + 1) + x + 1;
            INT32 index5 = (z + 1) * (m_meshCountCX + 1) + x + 1;

            // 三角形１の頂点
            pVertex[vertexCnt]      = m_pvModel[index0];
            pVertex[vertexCnt + 1]  = m_pvModel[index1];
            pVertex[vertexCnt + 2]  = m_pvModel[index2];

            // 三角形２の頂点
            pVertex[vertexCnt + 3]  = m_pvModel[index3];
            pVertex[vertexCnt + 4]  = m_pvModel[index4];
            pVertex[vertexCnt + 5]  = m_pvModel[index5];

            // 三角形１の法線ベクトル
            D3DXVECTOR3 v1 = m_pvModel[index1].vector - m_pvModel[index0].vector;
            D3DXVECTOR3 v2 = m_pvModel[index2].vector - m_pvModel[index0].vector;
            D3DXVECTOR3 vNormal;
            ::D3DXVec3Cross(&vNormal, &v1, &v2);        // 外積
            ::D3DXVec3Normalize(&vNormal, &vNormal);    // 正規化

            pVertex[vertexCnt].normal       = vNormal;
            pVertex[vertexCnt + 1].normal   = vNormal;
            pVertex[vertexCnt + 2].normal   = vNormal;

            // 法線ライン1
            D3DXVECTOR3 vCentric;
            ::D3DXVec3BaryCentric(&vCentric,
                                &m_pvModel[index0].vector,
                                &m_pvModel[index1].vector,
                                &m_pvModel[index2].vector,
                                0.333f,
                                0.333f);
            m_pNormalLine[normalLineCount].vector   = vCentric;
            m_pNormalLine[normalLineCount].color    = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
            normalLineCount++;
            m_pNormalLine[normalLineCount].vector   = vCentric + vNormal;
            m_pNormalLine[normalLineCount].color    = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
            normalLineCount++;

            // 三角形２の法線ベクトル
            v1 = m_pvModel[index4].vector - m_pvModel[index3].vector;
            v2 = m_pvModel[index5].vector - m_pvModel[index3].vector;
            ::D3DXVec3Cross(&vNormal, &v1, &v2);        // 外積
            ::D3DXVec3Normalize(&vNormal, &vNormal);    // 正規化

            pVertex[vertexCnt + 3].normal   = vNormal;
            pVertex[vertexCnt + 4].normal   = vNormal;
            pVertex[vertexCnt + 5].normal   = vNormal;

            // 法線ライン2
            vCentric;
            ::D3DXVec3BaryCentric(&vCentric,
                                &m_pvModel[index3].vector,
                                &m_pvModel[index4].vector,
                                &m_pvModel[index5].vector,
                                0.333f,
                                0.333f);
            m_pNormalLine[normalLineCount].vector   = vCentric;
            m_pNormalLine[normalLineCount].color    = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
            normalLineCount++;
            m_pNormalLine[normalLineCount].vector   = vCentric + vNormal;
            m_pNormalLine[normalLineCount].color    = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
            normalLineCount++;

            vertexCnt += 6;
        }
    }

    m_pD3DVB->Unlock();
    return  TRUE;
}

// メッシュのモデル座標設定（インデックスバッファで描画用）
BOOL Field::CreateIndexBuffer(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // オブジェクトのVertexBufferを生成
    if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(CUSTOMVERTEX) * (m_meshCountCX + 1) * (m_meshCountCY + 1),
                                            D3DUSAGE_WRITEONLY,
                                            CUSTOMVERTEX_FVF,
                                            D3DPOOL_MANAGED, &m_pD3DVB2, NULL)))
    {
        return FALSE;
    }

    // VertexBufferの中身を埋める
    CUSTOMVERTEX* pVertex;
    m_pD3DVB2->Lock(0, 0, (void**)&pVertex, 0);

    ::memcpy(pVertex, m_pvModel, sizeof(m_pvModel[0]) * (m_meshCountCX + 1) * (m_meshCountCY + 1));

    m_pD3DVB2->Unlock();

    // インデックスバッファーを生成
    if (FAILED(pD3DDevice->CreateIndexBuffer(GetPrimitiveCount() * 3 * 4,   // 32ビットインデックス
                                            D3DUSAGE_WRITEONLY,
                                            D3DFMT_INDEX32,
                                            D3DPOOL_MANAGED, &m_pD3DIdxB, NULL)))
    {
        return FALSE;
    }

    LPDWORD pIndex;
    m_pD3DIdxB->Lock(0, 0, (void**)&pIndex, 0);

    // メッシュループ
    INT32 vertexCnt = 0;
    for (INT32 z = 0; z < m_meshCountCY; z++)
    {
        for (INT32 x = 0; x < m_meshCountCX; x++)
        {
            INT32 index0 = z * (m_meshCountCX + 1) + x;
            INT32 index1 = (z + 1) * (m_meshCountCX + 1) + x + 1;
            INT32 index2 = (z + 1) * (m_meshCountCX + 1) + x;
            INT32 index3 = z * (m_meshCountCX + 1) + x;
            INT32 index4 = z * (m_meshCountCX + 1) + x + 1;
            INT32 index5 = (z + 1) * (m_meshCountCX + 1) + x + 1;

            // 三角形１
            pIndex[vertexCnt++] = index0;
            pIndex[vertexCnt++] = index1;
            pIndex[vertexCnt++] = index2;

            // 三角形２
            pIndex[vertexCnt++] = index3;
            pIndex[vertexCnt++] = index4;
            pIndex[vertexCnt++] = index5;
        }
    }

    m_pD3DIdxB->Unlock();
    return TRUE;
}

// ステータス文字列を取得
std::wstring Field::GetStatus() const
{
    TCHAR szFPS[1000];
    SPRINTF(szFPS, 1000,
            _T("ポリゴン数 ： %d\n")
            _T("バッファモード ： %s\n")
            _T("モデル位置 ： X=%f, Y=%f, Z=%f\n")
            _T("モデル回転 ： X=%f, Y=%f, Z=%f\n")
            _T("高さ単位 ： %f\n"),
            GetPrimitiveCount(),
            ((m_isDrawVB) ? _T("VertexBuffer"):_T("IndexBuffer")),
            m_moveX, m_moveY, m_moveZ,
            m_rotateX, m_rotateY, m_rotateZ,
            m_heightUnit);
    return szFPS;
}

/**
 * フィールドの破棄
 */
void Field::Release()
{
    SAFE_DELETE_ARRAY(m_pNormalLine);

    // 頂点バッファで描画用
    SAFE_RELEASE(m_pD3DVB);     // 頂点バッファの解放

    // インデックスバッファで描画用
    SAFE_RELEASE(m_pD3DIdxB);   // インデックスバッファの解放
    SAFE_RELEASE(m_pD3DVB2);    // 頂点バッファの解放

    // テクスチャの開放
    TextureManager::GetInstance()->ReleaseTexture(m_pTexture);
    m_pTexture = NULL;

    // テクスチャの開放
    TextureManager::GetInstance()->ReleaseTexture(m_pHeightMap);
    m_pHeightMap = NULL;
}

/**
 * 更新
 */
void Field::Update(float timeSpan)
{
    BOOL isCtrl = (::GetKeyState(VK_CONTROL) < 0);

    // モデル移動、回転
    if (::GetKeyState(VK_LEFT) < 0)
    {
        if (isCtrl)
        {
            m_rotateX += timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveX -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_RIGHT) < 0)
    {
        if (isCtrl)
        {
            m_rotateX -= timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveX += timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_DOWN) < 0)
    {
        if (isCtrl)
        {
            m_rotateY -= timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveY -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_UP) < 0)
    {
        if (isCtrl)
        {
            m_rotateY += timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveY += timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NEXT) < 0)
    {
        if (isCtrl)
        {
            m_rotateZ -= timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveZ -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_PRIOR) < 0)
    {
        if (isCtrl)
        {
            m_rotateZ += timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveZ += timeSpan * MOVE_SPEED;
        }
    }

    if (::GetKeyState('O') < 0)
    {
        m_heightUnit += timeSpan * 0.1f;
    }
    else if (::GetKeyState('L') < 0)
    {
        m_heightUnit -= timeSpan * 0.1f;
    }

    // 頂点バッファ or インデックスバッファ切り替え
    if (::GetKeyState(VK_F2) < 0)
    {
        // 初回のF2押下の場合
        if (!m_isF2)
        {
            m_isDrawVB = !m_isDrawVB;
        }
        m_isF2 = TRUE;
    }
    else
    {
        m_isF2 = FALSE;
    }

    // テクスチャ表示切り替え
    if (::GetKeyState(VK_F7) < 0)
    {
        // 初回のF7押下の場合
        if (!m_isF7)
        {
            m_isTexture = !m_isTexture;
        }
        m_isF7 = TRUE;
    }
    else
    {
        m_isF7 = FALSE;
    }

    // Fillモード使用可否切り替え
    if (::GetKeyState(VK_F5) < 0)
    {
        // 初回のF5押下の場合
        if (!m_isF5)
        {
            m_isSolid = !m_isSolid;
        }
        m_isF5 = TRUE;
    }
    else
    {
        m_isF5 = FALSE;
    }

    // 法線ラインの描画切り替え
    if (::GetKeyState(VK_F6) < 0)
    {
        // 初回のF6押下の場合
        if (!m_isF6)
        {
            m_isDrawNormal = !m_isDrawNormal;
        }
        m_isF6 = TRUE;
    }
    else
    {
        m_isF6 = FALSE;
    }
}

/** 
 * フィールドを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Field::Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps)
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

    // マテリアル
    pD3DDevice->SetMaterial(&m_D3DMaterial);

    // テクスチャの設定
    if (m_isTexture)
    {
        pD3DDevice->SetTexture(0, m_pTexture->pD3DTexture);
    }

    // 移動行列
    D3DXMATRIX mMove;
    D3DXMatrixIdentity(&mMove);     // 単位行列
    ::D3DXMatrixTranslation(&mMove, m_moveX, m_moveY, m_moveZ);

    // 回転行列
    D3DXMATRIX mRotateX;
    D3DXMatrixIdentity(&mRotateX);  // 単位行列
    ::D3DXMatrixRotationX(&mRotateX, D3DXToRadian(m_rotateX));

    D3DXMATRIX mRotateY;
    D3DXMatrixIdentity(&mRotateY);  // 単位行列
    ::D3DXMatrixRotationY(&mRotateY, D3DXToRadian(m_rotateY));

    D3DXMATRIX mRotateZ;
    D3DXMatrixIdentity(&mRotateZ);  // 単位行列
    ::D3DXMatrixRotationZ(&mRotateZ, D3DXToRadian(m_rotateZ));

    D3DXMATRIX mWorld;
    D3DXMatrixIdentity(&mWorld);    // 単位行列
    mWorld *= mRotateX; // X回転
    mWorld *= mRotateY; // Y回転
    mWorld *= mRotateZ; // Z回転
    mWorld *= mMove;    // 移動

    // ワールド行列設定
    pD3DDevice->SetTransform(D3DTS_WORLD, &mWorld);

    // Fillモード
    pD3DDevice->SetRenderState(D3DRS_FILLMODE, (m_isSolid) ? D3DFILL_SOLID : D3DFILL_WIREFRAME);
    //pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    //pD3DDevice->SetRenderState(D3DRS_SHADEMODE, (m_isSolid) ? D3DSHADE_FLAT : D3DSHADE_GOURAUD);

    // 頂点バッファを使用する場合
    if (m_isDrawVB)
    {
        // 頂点バッファの設定
        pD3DDevice->SetStreamSource(0, m_pD3DVB, 0, sizeof(CUSTOMVERTEX));

        // 頂点バッファのフォーマットの設定
        pD3DDevice->SetFVF(CUSTOMVERTEX_FVF);

        UINT32 startIndex = 0;
        UINT32 primitiveCount = GetPrimitiveCount();
        const UINT32 maxPrimitiveCount = pD3DCaps->MaxPrimitiveCount / 3;   // 頂点数のMAXを表すらしい
                                                                            // 三角リストなので、3で割る
        while (0 < primitiveCount)
        {
            INT32 drawPrimitiveCount =
                (maxPrimitiveCount < primitiveCount) ? maxPrimitiveCount : primitiveCount;
            // メッシュ状の描画
            pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,   // 三角リスト
                                    startIndex * 3,         // 三角リストなので、頂点数は3倍する
                                    drawPrimitiveCount);

            primitiveCount -= drawPrimitiveCount;
            startIndex += drawPrimitiveCount;
        }
    }
    // インデックスバッファを使用する場合
    else
    {
        // 頂点バッファの設定
        pD3DDevice->SetStreamSource(0, m_pD3DVB2, 0, sizeof(CUSTOMVERTEX));

        // 頂点バッファのフォーマットの設定
        pD3DDevice->SetFVF(CUSTOMVERTEX_FVF);

        // インデックスバッファの設定
        pD3DDevice->SetIndices(m_pD3DIdxB);

        // メッシュ状の描画
        pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                      (m_meshCountCX + 1) * (m_meshCountCY + 1),    // 頂点の数
                                      0,
                                      GetPrimitiveCount() );                        // プリミティブの数
    }

    // 元に戻す
    pD3DDevice->SetTexture(0, NULL);
    pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    // 法線の描画
    if (m_isDrawNormal)
    {
        // 頂点フォーマットの設定
        pD3DDevice->SetFVF(CUSTOMVERTEX_FVF_LINE);

        // 法線ラインの描画
        pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
                                    GetPrimitiveCount(),
                                    m_pNormalLine, sizeof(m_pNormalLine[0]));
    }
}
