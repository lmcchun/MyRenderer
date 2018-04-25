//
//  triangle.cpp
//  MyRenderer
//

#include "triangle.hpp"
#include "line.hpp"

using geometry::Vec2i;

namespace shape
{
	void triangle(geometry::Vec2i p0, geometry::Vec2i p1, geometry::Vec2i p2, image::TGA& image, const image::Color& color)
	{
		if (p0.y > p1.y)
		{
			std::swap(p0, p1);
		}
		if (p0.y > p2.y)
		{
			std::swap(p0, p2);
		}
		if (p1.y > p2.y)
		{
			std::swap(p1, p2);
		}
		// TODO
	}
}
