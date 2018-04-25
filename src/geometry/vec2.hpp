//
//  vec2.hpp
//  MyRenderer
//

#ifndef geometry_vec2_hpp
#define geometry_vec2_hpp

#include <cmath>
#include <iostream>

namespace geometry {
	template <typename T> struct Vec2
	{
		union
		{
			struct
			{
				T x;
				T y;
			};
			struct
			{
				T u;
				T v;
			};
			T raw[2];
		};
		Vec2() : x(0), y(0) {}
		Vec2(T x_, T y_) : x(x_), y(y_) {}
		inline Vec2<T> operator+(const Vec2<T> &V) const
		{
			return Vec2<T>(x + V.x, y + V.y);
		}
		inline Vec2<T> operator-(const Vec2<T> &V) const
		{
			return Vec2<T>(x - V.x, y - V.y);
		}
		inline Vec2<T> operator*(float f) const
		{
			return Vec2<T>(x * f, y * f);
		}
		template <typename> friend std::ostream& operator<<(std::ostream& s, Vec2<T>& v);
		template <typename> friend std::istream& operator>>(std::istream& s, Vec2<T>& v);
	};

	typedef Vec2<float> Vec2f;
	typedef Vec2<int> Vec2i;

	template <typename T> std::ostream& operator<<(std::ostream& s, Vec2<T>& v)
	{
		s << "(" << v.x << ", " << v.y << ")\n";
		return s;
	}

	template <typename T> std::istream& operator>>(std::istream& s, Vec2<T>& v)
	{
		s >> v.x >> v.y;
		return s;
	}
}

#endif /* geometry_vec2_hpp */
