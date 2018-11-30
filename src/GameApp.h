/**
 * @file
 * ゲームアプリケーションクラス宣言
 */
#ifndef __GAMEAPP_H__
#define __GAMEAPP_H__

#include "util/Fps.h"
class Scene;

/**
 * ゲームアプリケーションクラス
 */
class GameApp
{
    NO_COPY_CONSTRUCT(GameApp);
    NO_OPERATOR_EQUAL(GameApp);

private:
    // コンストラクタ
    GameApp();

public:
    // デストラクタ
    virtual ~GameApp();

    // シングルトンの初期化
    static void NewInstance();

    // シングルトンの開放
    static void DeleteInstance();

    // シングルトンインスタンスの取得
    static GameApp* GetInstance();

    // 初期処理
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

    // 終了処理
    void ExitInstance();

    // 実行
    int Run();

    // メッセージ処理
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // FPS管理の取得
    inline const Fps* GetFPS() const { return &m_Fps; }

private:
    // デバイス初期化
    BOOL InitDevice();

    // シーンを描画
    void DrawScene();

private:
    static LPCTSTR WINDOW_CLASS;    ///< 登録するウィンドウクラス名
    static GameApp* s_pInstance;    ///< シングルトンインスタンス保持

    HINSTANCE   m_hInstance;        ///< アプリケーションインスタンス
    HWND        m_hWnd;             ///< ウィンドウハンドル
    Scene*      m_pScene;           ///< シーン
    Fps         m_Fps;              ///< FPS管理

    LPDIRECT3D9             m_pD3D;         /// IDirect3D9インターフェイスへのポインタ
    LPDIRECT3DDEVICE9       m_pD3DDevice;   /// IDirect3DDevice9インターフェイスへのポインタ
    D3DPRESENT_PARAMETERS   m_D3DPP;        /// デバイスのプレゼンテーションパラメータ
    D3DCAPS9                m_D3DCaps;      /// デバイス情報
};

#endif // __GAMEAPP_H__
