/**
 * @file
 * 共通定義
 */

#ifndef _COMMON_H_
#define _COMMON_H_

// 三角関数
#define SIN(x)      sinf(x)
#define COS(x)      cosf(x)
#define TAN(x)      tanf(x)
#define ASIN(x)     asinf(x)
#define ACOS(x)     acosf(x)
#define ATAN(x)     atanf(x)
#define ATAN2(y,x)  atan2f(y,x)

// 平方根
#define SQRT(x)     sqrtf(x)

// 文字列操作
#define STRCPY(dst, dst_size, src)      _tcscpy_s(dst, dst_size, src)
#define STRCPY2(dst, src)               _tcscpy_s(dst, src)
#define SPRINTF(buff, buffsize, ...)    _stprintf_s(buff, buffsize, __VA_ARGS__)
#define STRLEN(str)                     _tcslen(str)    // ワイド文字の場合はバイト数ではなく文字数を返す
#define STRCAT(dst, dst_size, src)      _tcscat_s(dst, dst_size, src)

// 開放
#define SAFE_DELETE(p)          { if (p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p)    { if (p) { delete[] (p); (p) = NULL; } }
#define SAFE_RELEASE(p)         { if (p) { (p)->Release(); (p) = NULL; } }

// floatのビット状態を維持したままDWORDに変換
inline DWORD ToDWordBit(float value)
{
    return *(reinterpret_cast<DWORD*>(&value));
}

// 暗黙生成の防止用
#define NO_COPY_CONSTRUCT(class_name)   class_name(const class_name&)
#define NO_OPERATOR_EQUAL(class_name)   class_name& operator =(const class_name&)

// デバッグ用
#define ASSERT(m)       (assert(m))
#ifdef _DEBUG
    #define TRACE(m)    (printf(m))
    #define VERIFY(m)   (assert(m))
#else
    #define TRACE(m)    ((void)0)
    #define VERIFY(m)   (m)
#endif // _DEBUG

#endif //_COMMON_H_
