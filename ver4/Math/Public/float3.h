#pragma once

namespace DragonLib
{
    struct Float3
    {
        union
        {
            struct
            {
                float r, g, b;
            };
            struct
            {
                float x, y, z;
            };
            float f[3];
        };

        Float3() noexcept = default;
        Float3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
        Float3(float* a) noexcept : x(a[0]), y(a[1]), z(a[2]) {}

        operator float* ()
        {
            return f;
        }
    };
}

