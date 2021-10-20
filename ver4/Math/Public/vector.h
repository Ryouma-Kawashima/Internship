#pragma once

#include <intrin.h>
#include <memory.h>

namespace DragonLib
{
    struct Vector
    {
        __m128 vec;

        explicit Vector() noexcept = default;
        explicit Vector(float* a) noexcept
        {
            vec = _mm_load_ps(a);
        }
        explicit Vector(__m128 m) noexcept
        {
            memcpy(&vec, &m, sizeof(__m128));
        }

        operator float* ()
        {
            return vec.m128_f32;
        }
        operator const float* ()
        {
            return vec.m128_f32;
        }
        operator __m128 ()
        {
            return vec;
        }

        Vector& operator = (const Vector& other)
        {
            return *this = other;
        }
        Vector& operator = (const __m128& other)
        {
            memcpy(&vec, &other, sizeof(__m128));
            return *this;
        }

        Vector& operator += (const Vector& other)
        {
            return *this = _mm_add_ps(vec, other.vec);
        }
        Vector& operator -= (const Vector& other)
        {
            return *this = _mm_sub_ps(vec, other.vec);
        }
        Vector& operator *= (const Vector& other)
        {
            return *this = _mm_mul_ps(vec, other.vec);
        }
        Vector& operator /= (const Vector& other)
        {
            return *this = _mm_div_ps(vec, other.vec);
        }
        Vector& operator *= (float other)
        {
            return *this = _mm_mul_ps(vec, _mm_set_ps1(other));
        }
        Vector& operator /= (float other)
        {
            return *this = _mm_div_ps(vec, _mm_set_ps1(other));
        }

        Vector operator + (const Vector& other) const
        {
            return Vector(_mm_add_ps(vec, other.vec));
        }
        Vector operator - (const Vector& other) const
        {
            return Vector(_mm_sub_ps(vec, other.vec));
        }
        Vector operator * (const Vector& other) const
        {
            return Vector(_mm_mul_ps(vec, other.vec));
        }
        Vector operator / (const Vector& other) const
        {
            return Vector(_mm_div_ps(vec, other.vec));
        }
        Vector operator * (float other) const
        {
            return Vector(_mm_mul_ps(vec, _mm_set_ps1(other)));
        }
        Vector operator / (float other) const
        {
            return Vector(_mm_div_ps(vec, _mm_set_ps1(other)));
        }
    };
}
