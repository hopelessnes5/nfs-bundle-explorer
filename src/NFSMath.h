#ifndef __NFSMath_h__
#define __NFSMath_h__

#include <cmath>
#include <cstring>
#include <cassert>
#include "NFSTypes.h"

// Константы
inline constexpr float MY_PI       = 3.14159265358979323846f;
inline constexpr float MY_DEG2RAD  = MY_PI / 180.0f;
inline constexpr float MY_RAD2DEG  = 180.0f / MY_PI;
inline constexpr float MY_EPS      = 1e-6f;
inline constexpr float MY_INV255   = 1.0f / 255.0f;

// Быстрый и точный расчет для современных процессоров
inline float Rsqrt(float x)
{
    return 1.0f / std::sqrt(x);
}

// 16-битный float (half-precision)
struct float16
{
public:
    float16() : value(0) {}
    float16(float f) : value(Float32ToFloat16(f)) {}
    float16(const float16& other) = default;
    float16& operator=(const float16& other) = default;

    operator float() const
    {
        return Float16ToFloat32(value);
    }

    bool operator==(const float16& f) const { return IsEqual(f); }
    bool operator!=(const float16& f) const { return !IsEqual(f); }

private:
    bool IsEqual(const float16& f) const
    {
        // NaN check
        if (((value & 0x7C00) == 0x7C00 && (value & 0x03FF)) ||
            ((f.value & 0x7C00) == 0x7C00 && (f.value & 0x03FF)))
            return false;
        // +/- Zero
        if ((value & ~0x8000) == 0 && (f.value & ~0x8000) == 0)
            return true;
        return value == f.value;
    }

    static uint16 Float32ToFloat16(float x)
    {
        uint32 i = 0;
        std::memcpy(&i, &x, sizeof(float));

        int e = ((i >> 23) & 0xFF) - 112;
        int m = i & 0x007FFFFF;

        uint16 sh = (i >> 16) & 0x8000;
        if (e <= 0) {
            m = ((m | 0x00800000) >> (1 - e)) + 0x1000;
            sh |= (m >> 13);
        } else if (e == 143) {
            sh |= 0x7C00;
            if (m != 0) {
                m >>= 13;
                sh |= m | (m == 0);
            }
        } else {
            m += 0x1000;
            if (m & 0x00800000) {
                m = 0;
                e++;
            }
            if (e >= 31) {
                sh |= 0x7C00;
            } else {
                sh |= (e << 10) | (m >> 13);
            }
        }
        return sh;
    }

    static float Float16ToFloat32(uint16 sh)
    {
        uint32 s = (sh & 0x8000) << 16;
        uint32 e = (sh >> 10) & 0x1F;
        uint32 m = sh & 0x03FF;
        float result = 0.0f;

        if (e == 0) {
            if (m == 0) {
                std::memcpy(&result, &s, sizeof(float));
                return result;
            }
            while ((m & 0x0400) == 0) {
                m += m;
                e--;
            }
            e++;
            m &= ~0x0400;
        } else if (e == 31) {
            s |= 0x7F800000 | (m << 13);
            std::memcpy(&result, &s, sizeof(float));
            return result;
        }

        s |= ((e + 112) << 23) | (m << 13);
        std::memcpy(&result, &s, sizeof(float));
        return result;
    }

protected:
    uint16 value;
};

// Вектор 2D
template <typename T>
class _vec2
{
public:
    _vec2() : x(T(0)), y(T(0)) {}
    _vec2(T _x, T _y) : x(_x), y(_y) {}

    _vec2<T> operator * (float f) const { return _vec2<T>(x * f, y * f); }
    _vec2<T> operator / (float f) const { return _vec2<T>(x / f, y / f); }
    _vec2<T> operator + (const _vec2<T>& b) const { return _vec2<T>(x + b.x, y + b.y); }
    _vec2<T> operator - () const { return _vec2<T>(-x, -y); }
    _vec2<T> operator - (const _vec2<T>& b) const { return _vec2<T>(x - b.x, y - b.y); }

    _vec2<T>& operator *= (float f) { x *= f; y *= f; return *this; }
    _vec2<T>& operator /= (float f) { x /= f; y /= f; return *this; }
    _vec2<T>& operator += (const _vec2<T>& b) { x += b.x; y += b.y; return *this; }
    _vec2<T>& operator -= (const _vec2<T>& b) { x -= b.x; y -= b.y; return *this; }

    operator T*() { return v; }
    operator const T*() const { return v; }

    T& operator[](int i) { return v[i]; }
    const T& operator[](int i) const { return v[i]; }

    T length() const { return static_cast<T>(std::sqrt(x * x + y * y)); }

    void normalize()
    {
        T inv = static_cast<T>(Rsqrt(x * x + y * y));
        x *= inv;
        y *= inv;
    }

public:
    union {
        struct { T x, y; };
        T v[2];
    };
};

using vec2  = _vec2<float>;
using vec2f = _vec2<float>;
using vec2d = _vec2<double>;

// Вектор 3D
template <typename T>
class _vec3
{
public:
    static _vec3<T> Fromuint32(uint32 dw)
    {
        constexpr T inv255 = static_cast<T>(MY_INV255);
        return _vec3<T>(static_cast<T>((dw & 0x00ff0000) >> 16) * inv255,
                        static_cast<T>((dw & 0x0000ff00) >>  8) * inv255,
                        static_cast<T>((dw & 0x000000ff) >>  0) * inv255);
    }

    _vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    _vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

    _vec3<T> operator * (float f) const { return _vec3<T>(x * f, y * f, z * f); }
    _vec3<T> operator / (float f) const { return _vec3<T>(x / f, y / f, z / f); }
    _vec3<T> operator + (const _vec3<T>& b) const { return _vec3<T>(x + b.x, y + b.y, z + b.z); }
    _vec3<T> operator - () const { return _vec3<T>(-x, -y, -z); }
    _vec3<T> operator - (const _vec3<T>& b) const { return _vec3<T>(x - b.x, y - b.y, z - b.z); }

    _vec3<T>& operator *= (float f) { x *= f; y *= f; z *= f; return *this; }
    _vec3<T>& operator /= (float f) { x /= f; y /= f; z /= f; return *this; }
    _vec3<T>& operator += (const _vec3<T>& b) { x += b.x; y += b.y; z += b.z; return *this; }
    _vec3<T>& operator -= (const _vec3<T>& b) { x -= b.x; y -= b.y; z -= b.z; return *this; }

    operator T*() { return v; }
    operator const T*() const { return v; }

    T& operator[](int i) { return v[i]; }
    const T& operator[](int i) const { return v[i]; }

    T length() const { return static_cast<T>(std::sqrt(x * x + y * y + z * z)); }

    void normalize()
    {
        T inv = static_cast<T>(Rsqrt(x * x + y * y + z * z));
        x *= inv;
        y *= inv;
        z *= inv;
    }

public:
    union {
        struct { T x, y, z; };
        T v[3];
    };
};

template <typename T> inline T dot(const _vec3<T>& v1, const _vec3<T>& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T> inline _vec3<T> cross(const _vec3<T>& v1, const _vec3<T>& v2)
{
    return _vec3<T>(v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x);
}

using vec3  = _vec3<float>;
using vec3f = _vec3<float>;
using vec3d = _vec3<double>;

// Кватернион
template <typename T>
class _quat
{
public:
    union {
        struct { T x, y, z, w; };
        T v[4];
    };

    _quat() : x(T(0)), y(T(0)), z(T(0)), w(T(1)) {}
    _quat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

    operator T*() { return v; }
    operator const T*() const { return v; }

    T& operator[](int i) { return v[i]; }
    const T& operator[](int i) const { return v[i]; }

    _quat<T> operator * (float a) const { return _quat<T>(x * a, y * a, z * a, w * a); }
    _quat<T> operator * (const _quat<T>& q) const
    {
        return _quat<T>(w * q.x + x * q.w + y * q.z - z * q.y,
                        w * q.y + y * q.w + z * q.x - x * q.z,
                        w * q.z + z * q.w + x * q.y - y * q.x,
                        w * q.w - x * q.x - y * q.y - z * q.z);
    }

    _vec3<T> operator * (const _vec3<T>& p) const
    {
        float xxzz = x * x - z * z;
        float wwyy = w * w - y * y;
        float xw2 = x * w * 2.0f;
        float xy2 = x * y * 2.0f;
        float xz2 = x * z * 2.0f;
        float yw2 = y * w * 2.0f;
        float yz2 = y * z * 2.0f;
        float zw2 = z * w * 2.0f;

        return _vec3<T>(
            (xxzz + wwyy) * p.x + (xy2 + zw2) * p.y + (xz2 - yw2) * p.z,
            (xy2 - zw2) * p.x + (y * y + w * w - x * x - z * z) * p.y + (yz2 + xw2) * p.z,
            (xz2 + yw2) * p.x + (yz2 - xw2) * p.y + (wwyy - xxzz) * p.z
        );
    }

    _quat<T> operator - () const { return _quat<T>(-x, -y, -z, -w); }
    _quat<T> operator + (const _quat<T>& q) const { return _quat<T>(x + q.x, y + q.y, z + q.z, w + q.w); }

    _quat<T> inverse() const
    {
        T lenSq = x * x + y * y + z * z + w * w;
        if (lenSq > T(0)) {
            T invLenSq = T(1) / lenSq;
            return _quat<T>(-x * invLenSq, -y * invLenSq, -z * invLenSq, w * invLenSq);
        }
        assert(false);
        return _quat<T>(T(0), T(0), T(0), T(1));
    }

    _quat<T>& invert() { return *this = this->inverse(); }

    _quat<T>& normalize()
    {
        T len = static_cast<T>(std::sqrt(x * x + y * y + z * z + w * w));
        if (len) {
            T ilength = T(1) / len;
            x *= ilength; y *= ilength; z *= ilength; w *= ilength;
        }
        return *this;
    }

    T yaw()   const { return std::asin(-2 * (x * z - w * y)); }
    T pitch() const { return std::atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z); }
    T roll()  const { return std::atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z); }
};

using quat  = _quat<float>;
using quatf = _quat<float>;
using quatd = _quat<double>;

// Матрица 4x4
template <typename T>
class _mat4
{
public:
    _mat4()
    {
        mat[0] = 1.0f; mat[4] = 0.0f; mat[8]  = 0.0f; mat[12] = 0.0f;
        mat[1] = 0.0f; mat[5] = 1.0f; mat[9]  = 0.0f; mat[13] = 0.0f;
        mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = 1.0f; mat[14] = 0.0f;
        mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
    }

    _mat4<T>& set(const _quat<T>& q)
    {
        T x2 = q.x + q.x; T y2 = q.y + q.y; T z2 = q.z + q.z;
        T xx = q.x * x2;  T xy = q.x * y2;  T xz = q.x * z2;
        T yy = q.y * y2;  T yz = q.y * z2;  T zz = q.z * z2;
        T wx = q.w * x2;  T wy = q.w * y2;  T wz = q.w * z2;

        mat[0] = 1.0f - (yy + zz); mat[4] = xy + wz;         mat[8]  = xz - wy;
        mat[1] = xy - wz;         mat[5] = 1.0f - (xx + zz); mat[9]  = yz + wx;
        mat[2] = xz + wy;         mat[6] = yz - wx;         mat[10] = 1.0f - (xx + yy);

        mat[3]  = 0.0f; mat[7]  = 0.0f; mat[11] = 0.0f;
        mat[12] = 0.0f; mat[13] = 0.0f; mat[14] = 0.0f; mat[15] = 1.0f;
        return *this;
    }

    _mat4<T> operator * (const _mat4<T>& m) const
    {
        _mat4<T> ret;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                ret[i + j * 4] = mat[i] * m[j * 4] +
                                 mat[i + 4] * m[1 + j * 4] +
                                 mat[i + 8] * m[2 + j * 4] +
                                 mat[i + 12] * m[3 + j * 4];
            }
        }
        return ret;
    }

    _mat4<T>& operator *= (const _mat4<T>& m) { return *this = *this * m; }

    operator float*() { return mat; }
    operator const float*() const { return mat; }

    _mat4<T>& SetTranslateVector(const _vec3<T>& v)
    {
        mat[12] = v.x; mat[13] = v.y; mat[14] = v.z;
        return *this;
    }

    _vec3<T> GetTranslateVector() const
    {
        return _vec3<T>(mat[12], mat[13], mat[14]);
    }

    _vec3<T> TransformPoint(const _vec3<T>& b) const
    {
        return _vec3<T>(mat[0] * b.x + mat[4] * b.y + mat[8]  * b.z + mat[12],
                        mat[1] * b.x + mat[5] * b.y + mat[9]  * b.z + mat[13],
                        mat[2] * b.x + mat[6] * b.y + mat[10] * b.z + mat[14]);
    }

    _vec3<T> TransformDirection(const _vec3<T>& b) const
    {
        return _vec3<T>(mat[0] * b.x + mat[4] * b.y + mat[8]  * b.z,
                        mat[1] * b.x + mat[5] * b.y + mat[9]  * b.z,
                        mat[2] * b.x + mat[6] * b.y + mat[10] * b.z);
    }

public:
    union {
        struct {
            T m00, m10, m20, m30,
              m01, m11, m21, m31,
              m02, m12, m22, m32,
              m03, m13, m23, m33;
        };
        T mat[16];
    };
};

using mat4  = _mat4<float>;
using mat4f = _mat4<float>;
using mat4d = _mat4<double>;

#endif // __NFSMath_h__