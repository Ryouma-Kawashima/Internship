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
            float f[3];
        };

        operator float* ()
        {
            return f;
        }
    };
}

