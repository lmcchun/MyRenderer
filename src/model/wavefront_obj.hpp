//
//  wavefront_obj.hpp
//  MyRenderer
//

#ifndef model_wavefront_obj
#define model_wavefront_obj

#include <string>
#include <vector>
#include "../geometry/vec2.hpp"
#include "../geometry/vec3.hpp"
#include "../image/color.hpp"
#include "../image/tga.hpp"

namespace model {
    class WavefrontObj {
    public:
        WavefrontObj(std::string path);
        ~WavefrontObj();
        std::size_t vertice_number() const;
        std::size_t face_number() const;
        geometry::Vec3f vertex(std::size_t nth_vertex) const;
        std::vector<std::size_t> face(std::size_t nth_face) const;
        void draw(image::TGA &image, const image::Color& color) const;
    private:
        std::vector<geometry::Vec3f> vertices;
        std::vector<std::vector<geometry::Vec3<std::size_t>>> faces;
    };
}

#endif /* model_wavefront_obj */
