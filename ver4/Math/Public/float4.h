#pragma once

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
            float f[4];
        };

        operator float* ()
        {
            return f;
        }
    };
}
