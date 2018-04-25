//
//  line.hpp
//  MyRenderer
//

#ifndef line_hpp
#define line_hpp

#include "../geometry/vec2.hpp"
#include "../image/color.hpp"
#include "../image/tga.hpp"

namespace shape
{
	void line(geometry::Vec2i p0_, geometry::Vec2i p1_, image::TGA& image, const image::Color& color);
}

#endif /* line_hpp */
