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
        static_assert(IsSupported, "Vector4<T>にサポートされていない型が入りました。");
    };

    template <class T>
    class Vector4<T, true>
    {
        Vector4(const T* a);
        Vector4(T x, T y, T z, T w);

        // Casting
        operator T* ();
        operator const T* () const;

        #if !CAN_BE_USED_SIMD

        // Assignment
        Vector4<T>& operator += (const Vector4<T>& v);
        Vector4<T>& operator -= (const Vector4<T>& v);
        Vector4<T>& operator *= (const Vector4<T>& v);
        Vector4<T>& operator /= (const Vector4<T>& v);

        // Binary
        Vector4<T> operator + (const Vector4<T>& v) const;
        Vector4<T> operator - (const Vector4<T>& v) const;
        Vector4<T> operator * (const Vector4<T>& v) const;
        Vector4<T> operator / (const Vector4<T>& v) const;

        #endif
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

        // Calc
        static void Dot(float* out, Vector4<float>* v1, Vector4<float>* v2);
        static void Cross(Vector4<float>* out, Vector4<float>* v1, Vector4<float>* v2);
        static void Length(float* out, Vector4<float>* v);
        static void Normalize(Vector4<float>* out, Vector4<float>* v);

        #if CAN_BE_USED_SIMD
        Vector4(__m128 m) { this->m = m; }
        operator __m128* () { return &m; }

        // Assignment
        Vector4<float>& operator += (const Vector4<float>& v);
        Vector4<float>& operator -= (const Vector4<float>& v);
        Vector4<float>& operator *= (const Vector4<float>& v);
        Vector4<float>& operator /= (const Vector4<float>& v);

        // Binary
        Vector4<float> operator + (const Vector4<float>& v) const;
        Vector4<float> operator - (const Vector4<float>& v) const;
        Vector4<float> operator * (const Vector4<float>& v) const;
        Vector4<float> operator / (const Vector4<float>& v) const;
        #endif
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

        #if CAN_BE_USED_SIMD
        Vector4(__m256d m) { this->m = m; }
        operator __m256d* () { return &m; }

        // Assignment
        Vector4<double>& operator += (const Vector4<double>& v);
        Vector4<double>& operator -= (const Vector4<double>& v);
        Vector4<double>& operator *= (const Vector4<double>& v);
        Vector4<double>& operator /= (const Vector4<double>& v);

        // Binary
        Vector4<double> operator + (const Vector4<double>& v) const;
        Vector4<double> operator - (const Vector4<double>& v) const;
        Vector4<double> operator * (const Vector4<double>& v) const;
        Vector4<double> operator / (const Vector4<double>& v) const;
        #endif
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

        #if CAN_BE_USED_SIMD
        Vector4(__m128i m) { this->m = m; }
        operator __m128i* () { return &m; }

        // Assignment
        Vector4<uint32_t>& operator += (const Vector4<uint32_t>& v);
        Vector4<uint32_t>& operator -= (const Vector4<uint32_t>& v);
        Vector4<uint32_t>& operator *= (const Vector4<uint32_t>& v);
        Vector4<uint32_t>& operator /= (const Vector4<uint32_t>& v);

        // Binary
        Vector4<uint32_t> operator + (const Vector4<uint32_t>& v) const;
        Vector4<uint32_t> operator - (const Vector4<uint32_t>& v) const;
        Vector4<uint32_t> operator * (const Vector4<uint32_t>& v) const;
        Vector4<uint32_t> operator / (const Vector4<uint32_t>& v) const;
        #endif
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

        #if CAN_BE_USED_SIMD
        Vector4(__m256i m) { this->m = m; }
        operator __m256i* () { return &m; }

        // Assignment
        Vector4<uint64_t>& operator += (const Vector4<uint64_t>& v);
        Vector4<uint64_t>& operator -= (const Vector4<uint64_t>& v);
        Vector4<uint64_t>& operator *= (const Vector4<uint64_t>& v);
        Vector4<uint64_t>& operator /= (const Vector4<uint64_t>& v);

        // Binary
        Vector4<uint64_t> operator + (const Vector4<uint64_t>& v) const;
        Vector4<uint64_t> operator - (const Vector4<uint64_t>& v) const;
        Vector4<uint64_t> operator * (const Vector4<uint64_t>& v) const;
        Vector4<uint64_t> operator / (const Vector4<uint64_t>& v) const;
        #endif
    };
}
