#pragma once

#include <intrin.h>
#include <memory.h>
#include <ctgmath>

#include "float3.h"
#include "vector.h"

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

        Matrix() noexcept = default;
        Matrix(
            float _00, float _01, float _02, float _03,
            float _10, float _11, float _12, float _13,
            float _20, float _21, float _22, float _23,
            float _30, float _31, float _32, float _33)
        {
            row0 = _mm_setr_ps(_00, _01, _02, _03);
            row1 = _mm_setr_ps(_10, _11, _12, _13);
            row2 = _mm_setr_ps(_20, _21, _22, _23);
            row3 = _mm_setr_ps(_30, _31, _32, _33);
        }
        Matrix(float* a)
        {
            for (int i = 0; i < 4; i++)
            {
                m[i] = _mm_loadu_ps(a + 4 * i);
            }
        }
        Matrix(__m128* a)
        {
            memcpy(m, a, sizeof(__m128) * 4);
        }

        operator __m128* ()
        {
            return m;
        }

        Matrix& operator = (const Matrix& other)
        {
            memcpy(this, &other, sizeof(Matrix));
            return *this;
        }
        Matrix& operator *= (const Matrix& other)
        {
            Multiply(*this, *this, other);
            return *this;
        }
        Matrix operator * (const Matrix& other) const
        {
            Matrix tmp;
            Multiply(tmp, *this, other);
            return tmp;
        }

        static void Identity(Matrix& out)
        {
            out = Matrix(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
        }
        static void Transpose(Matrix& out, const Matrix& in)
        {
            __m128 tmp0, tmp1, tmp2, tmp3;

            tmp0 = _mm_shuffle_ps(in.row0, in.row1, 0x44);
            tmp2 = _mm_shuffle_ps(in.row0, in.row1, 0xEE);
            tmp1 = _mm_shuffle_ps(in.row2, in.row3, 0x44);
            tmp3 = _mm_shuffle_ps(in.row2, in.row3, 0xEE);

            out.row0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
            out.row1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
            out.row2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
            out.row3 = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
        }
        static void Inverse(Matrix& out, const Matrix& in)
        {
            // Reserved
        }
        static void Multiply(Matrix& out, const Matrix& m0, const Matrix& m1)
        {
            Matrix tmp;
            Transpose(tmp, m1);

            out.row0 = _mm_mul_ps(m0.row0, tmp.row0);
            out.row1 = _mm_mul_ps(m0.row1, tmp.row1);
            out.row2 = _mm_mul_ps(m0.row2, tmp.row2);
            out.row3 = _mm_mul_ps(m0.row3, tmp.row3);
        }
        static void MultiplyTranspose(Matrix& out, const Matrix& m0, const Matrix& m1)
        {
            Multiply(out, m0, m1);
            Transpose(out, out);
        }
        static void Translate(Matrix& out, const Float3& vec)
        {
            Identity(out);
            out.row0.m128_f32[3] = vec.x;
            out.row1.m128_f32[3] = vec.y;
            out.row2.m128_f32[3] = vec.z;
        }
        static void Rotate(Matrix& out, const Float3& vec)
        {
            Identity(out);
            out.row0.m128_f32[0] = cos(vec.y) * cos(vec.z);
            out.row0.m128_f32[1] = sin(vec.x) * sin(vec.y) * cos(vec.z) - cos(vec.x) * sin(vec.z);
            out.row0.m128_f32[2] = cos(vec.x) * sin(vec.y) * cos(vec.z) + sin(vec.x) * sin(vec.z);

            out.row1.m128_f32[0] = cos(vec.y) * sin(vec.z);
            out.row1.m128_f32[1] = sin(vec.x) * sin(vec.y) * sin(vec.z) + cos(vec.x) * cos(vec.z);
            out.row1.m128_f32[2] = cos(vec.x) * sin(vec.y) * sin(vec.z) - sin(vec.x) * cos(vec.z);

            out.row2.m128_f32[0] = -sin(vec.y);
            out.row2.m128_f32[1] = sin(vec.x) * cos(vec.y);
            out.row2.m128_f32[2] = cos(vec.x) * cos(vec.y);
        }
        static void Scale(Matrix& out, const Float3& vec)
        {
            Identity(out);
            out.row0.m128_f32[0] = vec.x;
            out.row1.m128_f32[1] = vec.y;
            out.row2.m128_f32[2] = vec.z;
        }

        // World Matrix
        static void Transform(Matrix& out, const Float3& position, const Float3& rotation, const Float3& scale)
        {
            Matrix t, r, s;
            Translate(t, position);
            Rotate(r, rotation);
            Scale(s, scale);
            out = s * r * t;
        }

        // View Matrix
        static void LookAtLH(Matrix& out, const Vector& eye, const Vector& at, const Vector& up)
        {
            Vector zAxis = Vector::Normalize(at - eye);
            Vector xAxis = Vector::Normalize(Vector::Cross(up, zAxis));
            Vector yAxis = Vector::Cross(zAxis, xAxis);

            out = Matrix(
                xAxis.x, yAxis.x, zAxis.x, 0,
                xAxis.y, yAxis.y, zAxis.y, 0,
                xAxis.z, yAxis.z, zAxis.z, 0,
                -Vector::Dot(xAxis, eye), -Vector::Dot(yAxis, eye), -Vector::Dot(zAxis, eye), 1);
        }
        static void LookAtRH(Matrix& out, const Vector& eye, const Vector& at, const Vector& up)
        {
            Vector zAxis = Vector::Normalize(eye - at);
            Vector xAxis = Vector::Normalize(Vector::Cross(up, zAxis));
            Vector yAxis = Vector::Cross(zAxis, xAxis);

            out = Matrix(
                xAxis.x, yAxis.x, zAxis.x, 0,
                xAxis.y, yAxis.y, zAxis.y, 0,
                xAxis.z, yAxis.z, zAxis.z, 0,
                -Vector::Dot(xAxis, eye), -Vector::Dot(yAxis, eye), -Vector::Dot(zAxis, eye), 1);
        }
        static void LookToLH(Matrix& out, const Vector& eye, const Vector& dir, const Vector& up)
        {
            LookAtLH(out, eye, eye + dir, up);
        }
        static void LookToRH(Matrix& out, const Vector& eye, const Vector& dir, const Vector& up)
        {
            LookAtRH(out, eye, eye + dir, up);
        }

        // Projection Matrix
        // Reference: https://biwanoie.tokyo/czprf/
        static void ProjectionLH(Matrix& out, float l, float r, float b, float t, float zn, float zf)
        {
            out = Matrix(
                2 * zn / (r - l),   0,                  0,                      0,
                0,                  2 * zn / (t - b),   0,                      0,
                (l + r) / (l - r),  (t + b) / (b - t),  zf / (zf - zn),         1,
                0,                  0,                  zn * zf / (zn - zf),    0);
        }
        static void ProjectionRH(Matrix& out, float l, float r, float b, float t, float zn, float zf)
        {
            out = Matrix(
                2 * zn / (r - l),   0,                  0,                      0,
                0,                  2 * zn / (t - b),   0,                      0,
                (l + r) / (r - l),  (t + b) / (t - b),  zf / (zn - zf),        -1,
                0,                  0,                  zn * zf / (zn - zf),    0);
        }
    };
}
