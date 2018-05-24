//
//  line.cpp
//  MyRenderer
//

#include "line.hpp"

using geometry::Vec2i;

namespace shape
{
	void line(Vec2i p0, Vec2i p1, image::TGA& image, const image::Color& color)
	{
		bool steep = false;
		// if the line is steep, we transpose the image
		if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y))
		{
			std::swap(p0.x, p0.y);
			std::swap(p1.x, p1.y);
			steep = true;
		}
		if (p0.x > p1.x) // make it left−to−right
		{
			std::swap(p0, p1);
		}
		int dx = p1.x - p0.x;
		int dy = p1.y - p0.y;
		int derror = std::abs(dy) * 2;
		int error = 0;
		int inc = p1.y > p0.y ? 1 : -1;
		int y = p0.y;
		for (int x = p0.x; x <= p1.x; ++x)
		{
			if (steep)
			{
				image.set(y, x, color);
			}
			else
			{
				image.set(x, y, color);
			}
			error += derror;
			if (error > dx)
			{
				y += inc;
				error -= dx * 2;
			}
		}
	}
}
