/**
 * @file
 * FPS管理の実装
 */
#include "StdAfx.h"
#include "Fps.h"

/**
 * コンストラクタ
 */
Fps::Fps(float fixFPS)
    :   m_isFixed(TRUE),        // 固定FPSフラグ
        m_fixFPS(fixFPS),       // 固定FPS値
        m_currentTime(0.0f),    
        m_drawTime(0.0f),       // 前回の描画時間
        m_fps(0.0f),            // 現在のFPS値
        m_measureTime(0.0f),    // 前回の測定時間
        m_countOfDraw(0)        // 描画回数
{
    // タイマ精度を指定
    VERIFY(::timeBeginPeriod(1) == TIMERR_NOERROR);

    m_drawTime = ::timeGetTime() / 1000.0f;
    m_measureTime = ::timeGetTime() / 1000.0f;
}

/**
 * デストラクタ
 */
Fps::~Fps()
{
    // タイマ精度を戻す
    VERIFY(::timeEndPeriod(1) == TIMERR_NOERROR);
}

/**
 * ループ毎の処理
 */
void Fps::Update()
{
    m_currentTime = ::timeGetTime() / 1000.0f;

    float timePeriod = m_currentTime - m_measureTime;

    // FPSを1秒間隔で測定
    if (1.0f <= timePeriod)
    {
        m_measureTime = m_currentTime;
        m_fps = m_countOfDraw / timePeriod;
        m_countOfDraw = 0;
    }
}

/**
 * 1フレーム経過したかを判定
 */
BOOL Fps::IsFrameProgressed()
{
    float timeSpan = m_currentTime - m_drawTime;
    return ((1.0f / m_fixFPS) <= timeSpan);
}

/**
 * 描画毎の処理
 */
void Fps::UpdateDraw()
{
    m_countOfDraw++;

    // 描画時間を保持
    m_drawTime = m_currentTime;
}
