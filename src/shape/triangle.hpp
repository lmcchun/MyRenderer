//
//  triangle.hpp
//  MyRenderer
//

#ifndef triangle_hpp
#define triangle_hpp

#include "../geometry/vec2.hpp"
#include "../image/color.hpp"
#include "../image/tga.hpp"

namespace shape
{
    void triangle(geometry::Vec2i p0, geometry::Vec2i p1, geometry::Vec2i p2, image::TGA& image, const image::Color& color);
}

#endif /* triangle_hpp */
