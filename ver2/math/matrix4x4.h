#pragma once

namespace DragonLib
{
    struct Matrix4x4
    {
        union
        {
            struct
            {
                float _00, _01, _02, _03;
                float _10, _11, _12, _13;
                float _20, _21, _22, _23;
                float _30, _31, _32, _33;
            };
            float a[4][4];
            __m128 m[4];
        };


    };
}
