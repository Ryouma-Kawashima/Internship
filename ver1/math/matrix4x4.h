#pragma once

namespace DragonLib
{
    // SIMD�֕ϊ����Ďg���ꍇ�̓A���C�����g��16�ɂ��Ƃ��K�v������
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

            float f[16];

            #if CAN_BE_USED_SIMD
            __m128 m[4];
            #endif
        };
    };

}
