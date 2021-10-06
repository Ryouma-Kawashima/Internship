#pragma once

namespace DragonLib
{
    // サポートされた型: float, double, uint32_t, uint64_t
    template
    <
        class T,
        bool IsSupported = 
            std::is_same<T, float>::value       ||
            std::is_same<T, double>::value      ||
            std::is_same<T, uint32_t>::value    ||
            std::is_same<T, uint64_t>::value
    >
    struct Vector4
    {
        static_assert(IsSupported, "Tにサポートされていない型が入りました。");
    };

    template <class T>
    class Vector4<T, true>
    {
        Vector4(const float* a);
        Vector4(T x, T y);

        // Calc
        static void Dot(Vector4<T>*out, Vector4<T>*v1, Vector4<T>*v2);
        static void Cross(Vector4<T>*out, Vector4<T>*v1, Vector4<T>*v2);
        static void Length(T * out, Vector4<T>*v);
        static void Normalize(T * out, Vector4<T>*v);

        // Casting
        operator T* ();
        operator const T* () const;

        // Assignment
        Vector4<T>& operator += (const Vector4<T>&v);
        Vector4<T>& operator -= (const Vector4<T>&v);
        Vector4<T>& operator *= (const Vector4<T>&v);
        Vector4<T>& operator /= (const Vector4<T>&v);

        // Binary
        Vector4<T> operator + (const Vector4<T>&v) const;
        Vector4<T> operator - (const Vector4<T>&v) const;
        Vector4<T> operator * (const Vector4<T>&v) const;
        Vector4<T> operator / (const Vector4<T>&v) const;
    };

    template <>
    class Vector4<float, true>
    {
        union
        {
            float x, y, z, w;
            float r, g, b, a;

            #if CAN_BE_USED_SIMD
            __m128 m; // float[4]
            #endif
        };
    };

    template <>
    class Vector4<double, true>
    {
        union
        {
            double x, y, z, w;
            double r, g, b, a;

            #if CAN_BE_USED_SIMD
            __m256d m; // double[4]
            #endif
        };
    };

    template <>
    class Vector4<uint32_t, true>
    {
        union
        {
            uint32_t x, y, z, w;
            uint32_t r, g, b, a;

            #if CAN_BE_USED_SIMD
            __m128i m; // uint32_t[4] 
            #endif
        };
    };

    template <>
    class Vector4<uint64_t, true>
    {
        union
        {
            uint32_t x, y, z, w;
            uint32_t r, g, b, a;

            #if CAN_BE_USED_SIMD
            __m256i m; // uint64_t[4] 
            #endif
        };
    };
}
