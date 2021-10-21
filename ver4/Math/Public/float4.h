#pragma once

#include <memory.h>

namespace DragonLib
{
    struct Float4
    {
        union
        {
            struct
            {
                float r, g, b, a;
            };
            struct
            {
                float x, y, z, w;
            };
            float f[4];
        };

        Float4() noexcept = default();
        Float4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
        Float4(float* a) noexcept : x(a[0]), y(a[1]), z(a[2]), w(a[3]) {}
        Float4(__m128 m) noexcept : x(m.m128_f32[0]), y(m.m128_f32[1]), z(m.m128_f32[2]), w(m.m128_f32[3]) {}

        operator float* ()
        {
            return f;
        }
    };
}
