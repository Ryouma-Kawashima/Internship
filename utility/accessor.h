#pragma once
// -----------------------------------
// Author: Ryouma Kawashima
// Create: 2021/05/04
// Update: 2021/10/04
// -----------------------------------

namespace DragonLib
{
    template <class T, class U>
    class Accessor
    {
    public:
        Accessor(U var) { m_var = var; }

    private:
        friend T; // Tはフレンドクラス
        U m_var;  // Uは変数の型

    public:
        // 読み取りのための演算子をオーバーロードして公開
        // 型変換演算子
        operator U() const noexcept { return m_var; }
        // アロー演算子
        U* operator ->() const noexcept { return &m_var; }
        // 間接参照演算子
        U& operator *() const noexcept { return m_var; }

    private:
        // 書き込みのための演算子をオーバーロードして制限
        // 単純代入演算子
        U& operator =(const U& var)& { return m_var = var; }
        U& operator =(U&& var) & noexcept { return m_var = var; }
        // 複合代入演算子
        U& operator +=  (const U& var) { return m_var += var; }
        U& operator -=  (const U& var) { return m_var -= var; }
        U& operator *=  (const U& var) { return m_var *= var; }
        U& operator /=  (const U& var) { return m_var /= var; }
        U& operator %=  (const U& var) { return m_var %= var; }
        U& operator <<= (const U& var) { return m_var <<= var; }
        U& operator >>= (const U& var) { return m_var >>= var; }
        U& operator &=  (const U& var) { return m_var &= var; }
        U& operator |=  (const U& var) { return m_var |= var; }
        U& operator ^=  (const U& var) { return m_var ^= var; }
        // 後置インクリメント / デクリメント
        U operator ++(int) { return m_var++; }
        U operator --(int) { return m_var--; }
        // 前置インクリメント / デクリメント
        U& operator ++() { return ++m_var; }
        U& operator --() { return --m_var; }
    };
}
