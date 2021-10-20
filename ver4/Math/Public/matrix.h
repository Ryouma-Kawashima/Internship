#pragma once

#include <intrin.h>

namespace DragonLib
{
    struct Matrix
    {
        union
        {
            struct
            {
                __m128 row0, row1, row2, row3;
            };
            __m128 m[4];
        };

        operator __m128* ()
        {
            return m;
        }

        Matrix& operator *= (const Matrix& other)
        {
            Multiply(this, this, &other);
            return *this;
        }
        Matrix operator * (const Matrix& other) const
        {
            Matrix tmp;
            Multiply(&tmp, this, &other);
            return tmp;
        }

        static void Transpose(Matrix* out, const Matrix* in)
        {
            __m128 tmp0, tmp1, tmp2, tmp3;

            tmp0 = _mm_shuffle_ps(in->row0, in->row1, 0x44);
            tmp2 = _mm_shuffle_ps(in->row0, in->row1, 0xEE);
            tmp1 = _mm_shuffle_ps(in->row2, in->row3, 0x44);
            tmp3 = _mm_shuffle_ps(in->row2, in->row3, 0xEE);

            out->row0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
            out->row1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
            out->row2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
            out->row3 = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
        }
        static void Multiply(Matrix* out, const Matrix* in1, const Matrix* in2)
        {
            Matrix tmp;
            Transpose(&tmp, in2);

            out->row0 = _mm_mul_ps(in1->row0, tmp.row0);
            out->row1 = _mm_mul_ps(in1->row1, tmp.row1);
            out->row2 = _mm_mul_ps(in1->row2, tmp.row2);
            out->row3 = _mm_mul_ps(in1->row3, tmp.row3);
        }
    };
}
