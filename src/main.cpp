//
//  main.cpp
//  MyRenderer
//

#include "geometry/vec2.hpp"
#include "image/color.hpp"
#include "image/tga.hpp"
#include "model/wavefront_obj.hpp"
#include "shape/line.hpp"
#include "shape/triangle.hpp"

using namespace geometry;
using namespace shape;

const image::Color white = image::Color(255, 255, 255, 255);
const image::Color red = image::Color(255, 0, 0, 255);
const image::Color green = image::Color(0, 255, 0, 255);
const image::Color blue = image::Color(0, 0, 255, 255);

int main(int argc, const char * argv[]) {
    image::TGA image{64, 64, image::TGA::RGB};
    //model::WavefrontObj wavefrontObj{"african_head.obj"};
    //wavefrontObj.draw(image, white);
    Vec2i p0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i p1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i p2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    shape::my_line(Vec2i(10, 50), Vec2i(50, 20), image, white);
    //shape::line(p1[0], p1[1], image, red);
    //shape::line(p2[0], p2[1], image, green);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
