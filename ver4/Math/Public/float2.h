#pragma once

namespace DragonLib
{
    struct Float2
    {
        union
        {
            struct
            {
                float r, g;
            };
            struct
            {
                float x, y;
            };
            float f[2];
        };

        Float2() noexcept = default;
        Float2(float x, float y) : x(x), y(y) {}
        Float2(float* a) : x(a[0]), y(a[1]) {}

        operator float* ()
        {
            return f;
        }
    };
}
