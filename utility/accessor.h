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
        friend T; // T�̓t�����h�N���X
        U m_var;  // U�͕ϐ��̌^

    public:
        // �ǂݎ��̂��߂̉��Z�q���I�[�o�[���[�h���Č��J
        // �^�ϊ����Z�q
        operator U() const noexcept { return m_var; }
        // �A���[���Z�q
        U* operator ->() const noexcept { return &m_var; }
        // �ԐڎQ�Ɖ��Z�q
        U& operator *() const noexcept { return m_var; }

    private:
        // �������݂̂��߂̉��Z�q���I�[�o�[���[�h���Đ���
        // �P��������Z�q
        U& operator =(const U& var)& { return m_var = var; }
        U& operator =(U&& var) & noexcept { return m_var = var; }
        // ����������Z�q
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
        // ��u�C���N�������g / �f�N�������g
        U operator ++(int) { return m_var++; }
        U operator --(int) { return m_var--; }
        // �O�u�C���N�������g / �f�N�������g
        U& operator ++() { return ++m_var; }
        U& operator --() { return --m_var; }
    };
}
