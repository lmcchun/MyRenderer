//
//  vec3.hpp
//  MyRenderer
//

#ifndef geometry_vec3_hpp
#define geometry_vec3_hpp

#include <cmath>
#include <iostream>

namespace geometry {
    template <typename T> struct Vec3 {
        union {
            struct {
                T x;
                T y;
                T z;
            };
            struct {
                T ivert;
                T iuv;
                T inorm;
            };
            T raw[3];
        };
        Vec3() : x(0), y(0), z(0) {}
        Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
        inline Vec3<T> operator^(const Vec3<T> &v) const {
            return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }
        inline Vec3<T> operator+(const Vec3<T> &v) const {
            return Vec3<T>(x + v.x, y + v.y, z + v.z);
        }
        inline Vec3<T> operator-(const Vec3<T> &v) const {
            return Vec3<T>(x - v.x, y - v.y, z - v.z);
        }
        inline Vec3<T> operator*(float f) const {
            return Vec3<T>(x * f, y * f, z * f);
        }
        inline T operator*(const Vec3<T> &v) const {
            return x * v.x + y * v.y + z * v.z;
        }
        float norm() const {
            return std::sqrt(x * x + y * y + z * z);
        }
        Vec3<T>& normalize(T l = 1) {
            *this = (*this) * (l / norm());
            return *this;
        }
        template <typename> friend std::ostream& operator<<(std::ostream& s, Vec3<T>& v);
        template <typename> friend std::istream& operator>>(std::istream& s, Vec3<T>& v);
    };
    
    typedef Vec3<float> Vec3f;
    typedef Vec3<int> Vec3i;
    
    template <typename T> std::ostream& operator<<(std::ostream& s, Vec3<T>& v) {
        s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
        return s;
    }
    
    template <typename T> std::istream& operator>>(std::istream& s, Vec3<T>& v) {
        s >> v.x >> v.y >> v.z;
        return s;
    }
}

#endif /* geometry_vec3_hpp */

