#pragma once

namespace DragonLib
{
    // SIMDへ変換して使う場合はアライメントを16にしとく必要がある
    #if CAN_BE_USED_SIMD
    __declspec(align(16))
    #endif
    struct Vector4
    {
        union
        {
            struct
            {
                float x, y, z, w;
            };
            struct
            {
                float r, g, b, a;
            };

            #if CAN_BE_USED_SIMD
            __m128 m; // float[4]
            #endif
        };

        Vector4(const float* a);
        Vector4(float xyzw);
        Vector4(float x, float y, float z, float w);
        #if CAN_BE_USED_SIMD
        Vector4(const __m128 m);
        Vector4(const __m128* m);
        #endif

        // Math
        static void Dot(float& out, Vector4& v0, Vector4& v1);
        static void Cross(Vector4& out, Vector4& v0, Vector4& v1);
        static void Length(float& out, Vector4& v);
        static void Normalize(Vector4& out, Vector4& v);

        // Casting
        operator float* ();
        operator const float* () const;
        #if CAN_BE_USED_SIMD
        operator __m128* ();
        #endif

        // Assignment
        Vector4& operator += (const Vector4& v);
        Vector4& operator -= (const Vector4& v);
        Vector4& operator *= (const Vector4& v);
        Vector4& operator /= (const Vector4& v);
        Vector4& operator *= (const float f);
        Vector4& operator /= (const float f);

        // Binary
        Vector4 operator + (const Vector4& v) const;
        Vector4 operator - (const Vector4& v) const;
        Vector4 operator * (const Vector4& v) const;
        Vector4 operator / (const Vector4& v) const;
        Vector4 operator * (const float f) const;
        Vector4 operator / (const float f) const;
    };
}
