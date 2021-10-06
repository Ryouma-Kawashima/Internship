#include "../utility/static_polymorphism.h"

#include "../utility//param.h"
#include "../system/config.h"

#include "simd.h"

#include "vector4.h"

#include <tgmath.h>

using namespace DragonLib;

template <class T>
Vector4<T, true>::Vector4<T, true>(const T* a)
{
    x = a[0];
    y = a[1];
    z = a[2];
    w = a[3];
}
template <class T>
Vector4<T, true>::Vector4<T, true>(T x, T y, T z, T w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

// Calc
template <class T>
void Vector4<T, true>::Dot(Vector4<T>* out, Vector4<T>* v1, Vector4<T>* v2)
{
    *out = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;
}
template <class T>
void Vector4<T, true>::Cross(T* out, Vector4<T>* v1, Vector4<T>* v2)
{
    *out = v1->y * v2->z 
}
template <class T>
void Vector4<T, true>::Length(T* out, Vector4<T>* v)
{
    *out = static_cast<T>(sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w));
}
template <class T> 
void Vector4<T, true>::Normalize(T* out, Vector4<T>* v)
{
    T len;
    Length(&len, v);
    *out = *v / len;
}

// Casting
template <class T> 
Vector4<T, true>::operator T* ()
{
    return &x;
}
template <class T> 
Vector4<T, true>::operator const T* () const
{
    return &x;
}


#if CAN_BE_USED_SIMD

// float
// Assignment
Vector4<float>& Vector4<float, true>::operator += (const Vector4<float>& v)
{
    m = _mm_add_ps(m, v.m);
}
Vector4<float>& Vector4<float, true>::operator -= (const Vector4<float>& v)
{
    m = _mm_sub_ps(m, v.m);
}
Vector4<float>& Vector4<float, true>::operator *= (const Vector4<float>& v)
{
    m = _mm_mul_ps(m, v.m);
}
Vector4<float>& Vector4<float, true>::operator /= (const Vector4<float>& v)
{
    m = _mm_div_ps(m, v.m);
}

// Binary
Vector4<float> Vector4<float, true>::operator + (const Vector4<float>& v) const
{
    return Vector4(_mm_add_ps(m, v.m));
}
Vector4<float> Vector4<float, true>::operator - (const Vector4<float>& v) const
{
    return Vector4(_mm_sub_ps(m, v.m));
}
Vector4<float> Vector4<float, true>::operator * (const Vector4<float>& v) const
{
    return Vector4(_mm_mul_ps(m, v.m));
}
Vector4<float> Vector4<float, true>::operator / (const Vector4<float>& v) const
{
    return Vector4(_mm_div_ps(m, v.m));
}


// double
// Assignment
Vector4<double>& Vector4<double, true>::operator += (const Vector4<double>& v)
{

}
Vector4<double>& Vector4<double, true>::operator -= (const Vector4<double>& v)
{

}
Vector4<double>& Vector4<double, true>::operator *= (const Vector4<double>& v)
{

}
Vector4<double>& Vector4<double, true>::operator /= (const Vector4<double>& v)
{

}

// Binary
Vector4<double> Vector4<double, true>::operator + (const Vector4<double>& v) const
{

}
Vector4<double> Vector4<double, true>::operator - (const Vector4<double>& v) const
{

}
Vector4<double> Vector4<double, true>::operator * (const Vector4<double>& v) const
{

}
Vector4<double> Vector4<double, true>::operator / (const Vector4<double>& v) const
{

}


// uint32_t
// Assignment
Vector4<uint32_t>& Vector4<uint32_t, true>::operator += (const Vector4<uint32_t>& v)
{

}
Vector4<uint32_t>& Vector4<uint32_t, true>::operator -= (const Vector4<uint32_t>& v)
{

}
Vector4<uint32_t>& Vector4<uint32_t, true>::operator *= (const Vector4<uint32_t>& v)
{

}
Vector4<uint32_t>& Vector4<uint32_t, true>::operator /= (const Vector4<uint32_t>& v)
{

}

// Binary
Vector4<uint32_t> Vector4<uint32_t, true>::operator + (const Vector4<uint32_t>& v) const
{

}
Vector4<uint32_t> Vector4<uint32_t, true>::operator - (const Vector4<uint32_t>& v) const
{

}
Vector4<uint32_t> Vector4<uint32_t, true>::operator * (const Vector4<uint32_t>& v) const
{

}
Vector4<uint32_t> Vector4<uint32_t, true>::operator / (const Vector4<uint32_t>& v) const
{

}


// uint64_t
// Assignment
Vector4<uint64_t>& Vector4<uint64_t, true>::operator += (const Vector4<uint64_t>& v)
{

}
Vector4<uint64_t>& Vector4<uint64_t, true>::operator -= (const Vector4<uint64_t>& v)
{

}
Vector4<uint64_t>& Vector4<uint64_t, true>::operator *= (const Vector4<uint64_t>& v)
{

}
Vector4<uint64_t>& Vector4<uint64_t, true>::operator /= (const Vector4<uint64_t>& v)
{

}

// Binary
Vector4<uint64_t> Vector4<uint64_t, true>::operator + (const Vector4<uint64_t>& v) const
{

}
Vector4<uint64_t> Vector4<uint64_t, true>::operator - (const Vector4<uint64_t>& v) const
{

}
Vector4<uint64_t> Vector4<uint64_t, true>::operator * (const Vector4<uint64_t>& v) const
{

}
Vector4<uint64_t> Vector4<uint64_t, true>::operator / (const Vector4<uint64_t>& v) const
{

}

#else

// Assignment
template <class T>
Vector4<T>& Vector4<T, true>::operator += (const Vector4<T>& v)
{

}
template <class T>
Vector4<T>& Vector4<T, true>::operator -= (const Vector4<T>& v)
{

}
template <class T>
Vector4<T>& Vector4<T, true>::operator *= (const Vector4<T>& v)
{

}
template <class T>
Vector4<T>& Vector4<T, true>::operator /= (const Vector4<T>& v)
{

}

// Binary
template <class T>
Vector4<T> Vector4<T, true>::operator + (const Vector4<T>& v) const
{

}
template <class T>
Vector4<T> Vector4<T, true>::operator - (const Vector4<T>& v) const
{

}
template <class T>
Vector4<T> Vector4<T, true>::operator * (const Vector4<T>& v) const
{

}
template <class T>
Vector4<T> Vector4<T, true>::operator / (const Vector4<T>& v) const
{

}

#endif

