#include "../utility/static_polymorphism.h"

#include "../utility//param.h"
#include "../system/config.h"

#include "simd.h"

#include "vector4.h"

#include <tgmath.h>

using namespace DragonLib;


#if CAN_BE_USED_SIMD

Vector4::Vector4(const float* a)
{
    m = _mm_loadr_ps(a);
}
Vector4::Vector4(float xyzw)
{
    m = _mm_set_ps1(xyzw);
}
Vector4::Vector4(float x, float y, float z, float w)
{
    m = _mm_set_ps(w, z, y, x);
}
Vector4::Vector4(__m128 m)
{
    this->m = m;
}

#else

Vector4::Vector4(const float* a)
{
    x = a[0];
    y = a[1];
    z = a[2];
    w = a[3];
}
Vector4::Vector4(float xyzw)
{
    x = xyzw;
    y = xyzw;
    z = xyzw;
    w = xyzw;
}
Vector4::Vector4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

#endif

// Calc
#if CAN_BE_USED_SIMD

void Vector4::Dot(float& out, Vector4& v1, Vector4& v2)
{
    #ifdef _INCLUDED_SMM // SSE3
    out = _mm_dp_ps(v1.m, v2.m, 0xFF).m128_i32[0];
    #else
    __m128 m = _mm_mul_ps(v1.m, v2.m);
    out = m.m128_f32[0] + m.m128_f32[1] + m.m128_f32[2] + m.m128_f32[3];
    #endif
}
void Vector4::Cross(Vector4& out, Vector4& v1, Vector4& v2)
{
    out.m = _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(v1.m, v1.m, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(v2.m, v2.m, _MM_SHUFFLE(3, 1, 0, 2))),
        _mm_mul_ps(_mm_shuffle_ps(v1.m, v1.m, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(v2.m, v2.m, _MM_SHUFFLE(3, 0, 2, 1))));
}
void Vector4::Length(float& out, Vector4& v)
{
    __m128 m = _mm_sqrt_ps(_mm_div_ps(v.m, v.m));
    out = m.m128_f32[0] + m.m128_f32[1] + m.m128_f32[2] + m.m128_f32[3];
}
void Vector4::Normalize(Vector4& out, Vector4& v)
{
    float len;
    Length(len, v);
    out.m = _mm_div_ps(v.m, _mm_set1_ps(len));
}

#else

void Vector4::Dot(float& out, Vector4& v1, Vector4& v2)
{
    out = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}
void Vector4::Cross(Vector4& out, Vector4& v1, Vector4& v2)
{
    out = Vector4(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
        0.0f);
}
void Vector4::Length(float& out, Vector4& v)
{
    out = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}
void Vector4::Normalize(Vector4& out, Vector4& v)
{
    float len;
    Length(len, v);
    out = v / len;
}

#endif

// Casting
Vector4::operator float* ()
{
    return &x;
}
Vector4::operator const float* () const
{
    return &x;
}

#if CAN_BE_USED_SIMD

Vector4::operator __m128* ()
{
    return &m;
}


// Assignment
Vector4& Vector4::operator += (const Vector4& v)
{
    m = _mm_add_ps(m, v.m);
    return *this;
}
Vector4& Vector4::operator -= (const Vector4& v)
{
    m = _mm_sub_ps(m, v.m);
    return *this;
}
Vector4& Vector4::operator *= (const Vector4& v)
{
    m = _mm_mul_ps(m, v.m);
    return *this;
}
Vector4& Vector4::operator /= (const Vector4& v)
{
    m = _mm_div_ps(m, v.m);
    return *this;
}
Vector4& Vector4::operator *= (const float f)
{
    m = _mm_mul_ps(m, _mm_set1_ps(f));
    return *this;
}
Vector4& Vector4::operator /= (const float f)
{
    m = _mm_div_ps(m, _mm_set1_ps(f));
    return *this;
}

// Binary
Vector4 Vector4::operator + (const Vector4& v) const
{
    return Vector4(_mm_add_ps(m, v.m));
}
Vector4 Vector4::operator - (const Vector4& v) const
{
    return Vector4(_mm_sub_ps(m, v.m));
}
Vector4 Vector4::operator * (const Vector4& v) const
{
    return Vector4(_mm_mul_ps(m, v.m));
}
Vector4 Vector4::operator / (const Vector4& v) const
{
    return Vector4(_mm_div_ps(m, v.m));
}
Vector4 Vector4::operator * (const float f) const
{
    return Vector4(_mm_mul_ps(m, _mm_set1_ps(f)));
}
Vector4 Vector4::operator / (const float f) const
{
    return Vector4(_mm_mul_ps(m, _mm_set1_ps(f)));
}

#else

// Assignment
Vector4& Vector4::operator += (const Vector4& v)
{
    return *this = Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}
Vector4& Vector4::operator -= (const Vector4& v)
{
    return *this = Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}
Vector4& Vector4::operator *= (const Vector4& v)
{
    return *this = Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}
Vector4& Vector4::operator /= (const Vector4& v)
{
    return *this = Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}
Vector4& Vector4::operator *= (const float f)
{
    return *this = Vector4(x * f, y * f, z * f, w * f);
}
Vector4& Vector4::operator /= (const float f)
{
    return *this = Vector4(x / f, y / f, z / f, w / f);
}

// Binary
Vector4 Vector4::operator + (const Vector4& v) const
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}
Vector4 Vector4::operator - (const Vector4& v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}
Vector4 Vector4::operator * (const Vector4& v) const
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}
Vector4 Vector4::operator / (const Vector4& v) const
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}
Vector4 Vector4::operator * (const float f) const
{
    return Vector4(x * f, y * f, z * f, w * f);
}
Vector4 Vector4::operator / (const float f) const
{
    return Vector4(x / f, y / f, z / f, w / f);
}

#endif

