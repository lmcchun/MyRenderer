#ifndef image_color_hpp
#define image_color_hpp

namespace image
{
	struct Color
	{
		union
		{
			struct
			{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			};
			unsigned char raw[4];
			unsigned int value;
		};

		int bytespp;

		Color() : value(0), bytespp(1) {}

		Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : g(G), r(R), b(B), a(A), bytespp(4) {}

		Color(int v, int bpp) : value(v), bytespp(bpp) {}

		Color(const Color& c) : value(c.value), bytespp(c.bytespp) {}

		Color(const unsigned char* p, int bpp) : value(0), bytespp(bpp)
		{
			for (int i = 0; i < bpp; ++i)
			{
				raw[i] = p[i];
			}
		}

		Color& operator=(const Color& c)
		{
			if (this != &c)
			{
				bytespp = c.bytespp;
				value = c.value;
			}
			return *this;
		}
	};
}

#endif // image_color_hpp
