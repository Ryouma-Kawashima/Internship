#pragma once

namespace DragonLib
{
    // SIMDへ変換して使う場合はアライメントを16にしとく必要がある
    #if CAN_BE_USED_SIMD
    __declspec(align(16))
    #endif
    struct Matrix4x4
    {
        union
        {
            float   m00, m01, m02, m03,
                    m10, m11, m12, m13,
                    m20, m21, m22, m23,
                    m30, m31, m32, m33;

            #if CAN_BE_USED_SIMD

            #if defined(_M_X64)
            __m256 m[2];
            #elif defined(_M_IX86)
            __m128 m[4];
            #endif 

            #endif
        };

        Matrix4x4(
            float _00, float _01, float m02, float _03,
            float _10, float _11, float m12, float _13,
            float _20, float _21, float m22, float _23,
            float _30, float _31, float m32, float _33);

        Matrix4x4(const float a);

        #if CAN_BE_USED_SIMD

        #if defined(_M_X64)
        Matrix4x4(__m256* m0, __m256* m1);
        #elif defined(_M_IX86)
        Matrix4x4(__m128* m0, __m128* m1, __m128* m2, __m128* m3);
        #endif 

        #endif

        // Math


    };
}
