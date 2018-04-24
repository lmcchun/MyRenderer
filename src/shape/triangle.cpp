//
//  triangle.cpp
//  MyRenderer
//

#include "triangle.hpp"
#include "line.hpp"

using geometry::Vec2i;

namespace shape
{
    void triangle(geometry::Vec2i p0, geometry::Vec2i p1, geometry::Vec2i p2, image::TGA& image, const image::Color& color) {
        line(p0, p1, image, color);
        line(p1, p2, image, color);
        line(p2, p0, image, color);
    }
}
