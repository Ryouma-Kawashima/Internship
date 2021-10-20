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
            float f[2];
        };

        operator float* ()
        {
            return f;
        }
    };
}
