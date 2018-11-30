/**
 * @file
 * FPS管理の宣言
 */
#ifndef __FPS_H__
#define __FPS_H__

/**
 * FPS管理クラス
 *
 * 次の機能を提供@n
 * ・固定FPSのフレーム間隔時間判定@n
 * ・FPS値の計測@n
 * ・固定FPS、非固定FPS切り替え@n
 */
class Fps
{
    NO_COPY_CONSTRUCT(Fps);
    NO_OPERATOR_EQUAL(Fps);

public:
    // コンストラクタ
    Fps(float fixFPS);

    // デストラクタ
    virtual ~Fps();

public:
    // ループ毎の処理
    void Update();

    // 描画毎の処理
    void UpdateDraw();

    // 1フレーム経過したかを判定
    BOOL IsFrameProgressed();

public:
    // 固定FPSフラグを取得
    inline BOOL  IsFixedFPS() const { return m_isFixed; }

    // 固定FPSフラグを設定
    inline void SetFixedFPS(BOOL bFix) { m_isFixed = bFix; }

    // FPSを取得
    inline float GetFPS() const { return m_fps; }

    // 1フレームで経過した時間を取得
    inline float GetFrameTimeSpan() const { return m_currentTime - m_drawTime; }

private:
    BOOL    m_isFixed;      ///< 固定FPSフラグ
    float   m_fixFPS;       ///< 固定FPS値
    float   m_fps;          ///< 現在のFPS値
    float   m_currentTime;  ///< 現在の時間

    // フレーム間隔用
    float   m_drawTime;     ///< 前回の描画時間

    // FPSの測定用
    float   m_measureTime;  ///< 前回の測定時間
    UINT32  m_countOfDraw;  ///< 描画回数
};

#endif // __FPS_H__
