#ifndef image_header_hpp
#define image_header_hpp

namespace image {
#pragma pack(push,1)
    struct Header {
        char id_length;
        char color_map_type;
        char data_type_code;
        short color_map_origin;
        short color_map_length;
        char color_map_depth;
        short x_origin;
        short y_origin;
        short width;
        short height;
        char bits_per_pixel;
        char image_descriptor;
    };
#pragma pack(pop)
}

#endif // image_header_hpp
