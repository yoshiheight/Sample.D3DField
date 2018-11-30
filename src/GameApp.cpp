/**
 * @file
 * ゲームアプリケーションクラス定義
 */
#include "stdafx.h"
#include "resource.h"

#include "GameApp.h"
#include "scene/Scene.h"
//#include "util/PointSprite.h"

GameApp* GameApp::s_pInstance = NULL;
LPCTSTR GameApp::WINDOW_CLASS = _T("D3D_FIELD");

// エントリポイント
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     int nCmdShow)
{
    // シングルトン初期化
    GameApp::NewInstance();
    TextureManager::NewInstance();
//  PointSprite::NewInstance();

    // 前処理
    GameApp::GetInstance()->InitInstance(hInstance, nCmdShow);

    // 実行
    int result = GameApp::GetInstance()->Run();

    // 後処理
    GameApp::GetInstance()->ExitInstance();

    // シングルトン開放
//  PointSprite::DeleteInstance();
    TextureManager::DeleteInstance();
    GameApp::DeleteInstance();

//  _CrtDumpMemoryLeaks();

    return result;
}

// ウィンドウメッセージ処理
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return GameApp::GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

/**
 * コンストラクタ
 */
GameApp::GameApp()
    :   m_Fps(60.0f),
        m_hWnd(NULL),
        m_hInstance(NULL),
        m_pScene(NULL),
        m_pD3D(NULL),
        m_pD3DDevice(NULL)
{
}

/**
 * デストラクタ
 */
GameApp::~GameApp()
{
}

/**
 * シングルトンの初期化
 */
void GameApp::NewInstance()
{
    ASSERT(s_pInstance == NULL);
    s_pInstance = new GameApp();
}

/**
 * シングルトンの開放
 */
void GameApp::DeleteInstance()
{
    SAFE_DELETE(s_pInstance);
}

/**
 * シングルトンインスタンスを取得
 * @return シングルトンインスタンス
 */
GameApp* GameApp::GetInstance()
{
    return s_pInstance;
}

/**
 * 初期処理
 * @param hInstance インスタンスハンドル
 * @param nCmdShow 表示指定
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL GameApp::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // ウィンドウ クラスを登録
    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = ::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3D_FIELD));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = WINDOW_CLASS;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    ::RegisterClassEx(&wcex);

    // インスタンスハンドルを保持
    m_hInstance = hInstance;

    // ウィンドウ作成
    m_hWnd = CreateWindow(WINDOW_CLASS, _T("D3D_FIELD"), WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (m_hWnd == NULL)
    {
        return FALSE;
    }

    // ファイルドロップを許可
    ::DragAcceptFiles(m_hWnd, TRUE);

    // ウインドウサイズ設定
    ::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 1200, 1000, SWP_NOMOVE);

    // デバイスの初期化
    if (!InitDevice())
    {
        return FALSE;
    }

    // シーンの初期化
    m_pScene = new Scene();
    //if (!m_pScene->Create(m_pD3DDevice, NULL))
    if (!m_pScene->Create(m_pD3DDevice, _T("resource/heightmap.bmp")))
    {
        return FALSE;
    }

    // ウィンドウ表示
    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);

    return TRUE;
}

/**
 * 終了処理
 */
void GameApp::ExitInstance()
{
    SAFE_DELETE(m_pScene);
    SAFE_RELEASE(m_pD3DDevice);
    SAFE_RELEASE(m_pD3D);
}

/**
 * ゲーム実行
 */
int GameApp::Run()
{
    MSG msg;
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_Fps.Update();

            // 固定FPSの場合
            if (m_Fps.IsFixedFPS())
            {
                // フレーム間隔時間が経過した場合
                if (m_Fps.IsFrameProgressed())
                {
                    //if (::GetForegroundWindow() == stMsg.hwnd)
                    //{
                    //}

                    // 更新処理
                    m_pScene->Update(m_Fps.GetFrameTimeSpan());

                    // 描画
                    DrawScene();
                    m_Fps.UpdateDraw();
                }
            }
            // 非固定FPSの場合
            else
            {
                // 更新処理
                m_pScene->Update(m_Fps.GetFrameTimeSpan());

                // 描画
                DrawScene();
                m_Fps.UpdateDraw();
            }
        }

        if (m_Fps.IsFixedFPS())
        {
            // 適度なスリープ（ウィンドウアクティブ状態でスリープ量を可変にするとより最適）
            ::Sleep(1);
        }
    }

    return (int) msg.wParam;
}

/**
 * ウィンドウメッセージを処理
 * @param hWnd ウィンドウハンドル
 * @param message メッセージ
 * @param wParam Wパラメータ
 * @param lParam Lパラメータ
 * @return 実行結果
 */
LRESULT GameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    // ファイルドロップ
    case WM_DROPFILES:
        // 1番目のファイルパスを取得
        TCHAR szFileName[FILENAME_MAX];
        ::DragQueryFile((HDROP) wParam, 0, szFileName, FILENAME_MAX);
        m_pScene->Create(m_pD3DDevice, szFileName);
        break;
    // キーダウン
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hWnd);
        }
        else if (wParam == VK_F1)
        {
            // 固定FPS切り替え
            m_Fps.SetFixedFPS(!m_Fps.IsFixedFPS());
        }
        break;
    // 描画
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // シーンを描画
        DrawScene();

        EndPaint(hWnd, &ps);
        break;
    // ウィンドウ破棄
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    // デフォルト処理
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

/** 
 * 初期化
 * @param hWnd ウィンドウハンドル
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL GameApp::InitDevice()
{
    // Direct3D9オブジェクトの作成
    if ((m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0)
    {
        return FALSE;
    }

    // 現在のディスプレイモードを取得
    D3DDISPLAYMODE d3ddm;
    if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return FALSE;
    }

    // デバイスのプレゼンテーションパラメータを初期化
    ::ZeroMemory(&m_D3DPP, sizeof(m_D3DPP));
    m_D3DPP.BackBufferCount         = 1;
    m_D3DPP.Windowed                = TRUE;                     // ウインドウ内表示の指定
    m_D3DPP.BackBufferFormat        = d3ddm.Format;             // フォーマット指定
    m_D3DPP.SwapEffect              = D3DSWAPEFFECT_DISCARD;
    m_D3DPP.EnableAutoDepthStencil  = TRUE;                     // Ｚバッファを有効
    m_D3DPP.AutoDepthStencilFormat  = D3DFMT_D16;               // 自動深度ステンシルサーフェイスのフォーマット
    m_D3DPP.PresentationInterval    = D3DPRESENT_INTERVAL_IMMEDIATE; // VSync待ちしない

    // ディスプレイアダプタを表すためのデバイスを作成
    // 描画と頂点処理をハードウェアで行なう
    if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                                    D3DDEVTYPE_HAL, m_hWnd,
                                    D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                    &m_D3DPP, &m_pD3DDevice)))
    {
        // 上記の設定が失敗したら
        // 描画をハードウェアで行い、頂点処理はCPUで行なう
        if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                                        D3DDEVTYPE_HAL, m_hWnd,
                                        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                        &m_D3DPP, &m_pD3DDevice)))
        {
            // 上記の設定が失敗したら
            // 描画と頂点処理をCPUで行なう
            if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                                            D3DDEVTYPE_REF, m_hWnd,
                                            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                            &m_D3DPP, &m_pD3DDevice)))
            {
                return FALSE;
            }
        }
    }

    // デバイス情報取得
    if (FAILED(m_pD3DDevice->GetDeviceCaps(&m_D3DCaps)))
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * シーンの描画
 * @param pScene 描画するシーンオブジェクト
 */
void GameApp::DrawScene()
{
    // ビューポートと深度バッファのクリアとステンシルバッファの削除
    if (FAILED(m_pD3DDevice->Clear(0, NULL,             // クリアする領域は全面
                                D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // バックバッファ、深度バッファ（Zバッファ）を指定
                                D3DCOLOR_XRGB(0,0,100), // クリアする色
                                1.0f,                   // クリアする深度バッファ（Zバッファ）の値
                                0)))                    // クリアするステンシルバッファの値
    {
        return;
    }

    // 描画開始
    if (SUCCEEDED(m_pD3DDevice->BeginScene()))
    {
        // シーンの描画
        m_pScene->Draw(m_pD3DDevice, &m_D3DCaps);

        // 描画終了
        m_pD3DDevice->EndScene();
    }

    // 描画結果の転送
    if (FAILED(m_pD3DDevice->Present(0, 0, 0, 0)))
    {
        // デバイス消失から復帰
        m_pD3DDevice->Reset(&m_D3DPP);
    }
}
