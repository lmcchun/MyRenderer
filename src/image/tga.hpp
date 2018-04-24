#ifndef image_tga_hpp
#define image_tga_hpp

#include <fstream>
#include "color.hpp"

namespace image {
    class TGA {
    public:
        enum Format {
            GRAYSCALE = 1,
            RGB = 3,
            RGBA = 4
        };
        
        TGA();
        TGA(int w, int h, int bpp);
        TGA(const TGA& image);
        TGA& operator=(const TGA& image);
        ~TGA();
        bool read_tga_file(const char* filename);
        bool write_tga_file(const char* filename, bool rle = true);
        bool flip_horizontally();
        bool flip_vertically();
        bool scale(int w, int h);
        Color get(int x, int y);
        bool set(int x, int y, Color c);
        int get_width();
        int get_height();
        int get_bytespp();
        unsigned char* buffer();
        void clear();
    protected:
        unsigned char* data;
        int width;
        int height;
        int bytespp;
        
        bool load_rle_data(std::ifstream& in);
        bool unload_rle_data(std::ofstream& out);
    };
}

#endif // image_tga_hpp

