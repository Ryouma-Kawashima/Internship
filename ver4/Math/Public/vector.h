#pragma once

#include <intrin.h>
#include <memory.h>

namespace DragonLib
{
    struct Vector
    {
        union
        {
            struct alignas(16)
            {
                float w, z, y, x;
            };
            __m128 m;
        };
        
        Vector() noexcept = default;
        Vector(float x, float y, float z, float w = 0.0f)
        {
            m = _mm_setr_ps(x, y, z, w);
        }
        Vector(float* a) noexcept
        {
            m = _mm_loadu_ps(a);
            m.m128_f32[3] = 0.0f;
        }
        Vector(__m128 m) noexcept
        {
            memcpy(&this->m, &m, sizeof(__m128));
        }

        static float Length(const Vector& a)
        {
            __m128 len = _mm_sqrt_ps(a.m);
            return len.m128_f32[0] + len.m128_f32[1] + len.m128_f32[2] + len.m128_f32[3];
        }
        static Vector Normalize(const Vector& a)
        {
            float len = Length(a);
            return Vector(_mm_div_ps(a.m, _mm_set_ps1(len)));
        }
        static float Dot(const Vector& a, const Vector& b)
        {
            __m128 tmp = _mm_add_ps(a.m, b.m);
            return tmp.m128_f32[0] + tmp.m128_f32[1] + tmp.m128_f32[2] + tmp.m128_f32[3];
        }
        static Vector Cross(const Vector& a, const Vector& b)
        {
            __m128 tmp0, tmp1, tmp2, tmp3;

            tmp0 = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(3, 0, 2, 1));
            tmp1 = _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(3, 1, 0, 2));
            tmp2 = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(3, 1, 0, 2));
            tmp3 = _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(3, 0, 2, 1));

            return Vector(_mm_sub_ps(
                _mm_mul_ps(tmp0, tmp1),
                _mm_mul_ps(tmp2, tmp3)));
        }
        static Vector Cross(const Vector& a, const Vector& b, const Vector& c)
        {
            Vector v0 = b - a;
            Vector v1 = c - a;
            return Cross(v0, v1);
        }

        float Length()
        {
            return Length(*this);
        }
        Vector Normalize()
        {
            return Normalize(*this);
        }

        operator float* ()
        {
            return m.m128_f32;
        }
        operator const float* ()
        {
            return m.m128_f32;
        }
        operator __m128 ()
        {
            return m;
        }

        Vector& operator = (const Vector& other)
        {
            return *this = other;
        }
        Vector& operator = (const __m128& other)
        {
            memcpy(&m, &other, sizeof(__m128));
            return *this;
        }

        Vector& operator += (const Vector& other)
        {
            return *this = _mm_add_ps(m, other.m);
        }
        Vector& operator -= (const Vector& other)
        {
            return *this = _mm_sub_ps(m, other.m);
        }
        Vector& operator *= (const Vector& other)
        {
            return *this = _mm_mul_ps(m, other.m);
        }
        Vector& operator /= (const Vector& other)
        {
            return *this = _mm_div_ps(m, other.m);
        }
        Vector& operator *= (float other)
        {
            return *this = _mm_mul_ps(m, _mm_set_ps1(other));
        }
        Vector& operator /= (float other)
        {
            return *this = _mm_div_ps(m, _mm_set_ps1(other));
        }

        Vector operator + (const Vector& other) const
        {
            return Vector(_mm_add_ps(m, other.m));
        }
        Vector operator - (const Vector& other) const
        {
            return Vector(_mm_sub_ps(m, other.m));
        }
        Vector operator * (const Vector& other) const
        {
            return Vector(_mm_mul_ps(m, other.m));
        }
        Vector operator / (const Vector& other) const
        {
            return Vector(_mm_div_ps(m, other.m));
        }
        Vector operator * (float other) const
        {
            return Vector(_mm_mul_ps(m, _mm_set_ps1(other)));
        }
        Vector operator / (float other) const
        {
            return Vector(_mm_div_ps(m, _mm_set_ps1(other)));
        }

        Vector operator + () const
        {
            return *this;
        }
        Vector operator - () const
        {
            return Vector(_mm_mul_ps(m, _mm_set_ps1(-1.0f)));
        }
    };
}
