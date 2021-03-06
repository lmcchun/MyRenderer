#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>
#include <cmath>
#include "tga.hpp"
#include "header.hpp"

namespace image
{
	TGA::TGA() : data(NULL), width(0), height(0), bytespp(0)
	{
		//
	}

	TGA::TGA(int w, int h, int bpp) : data(NULL), width(w), height(h), bytespp(bpp)
	{
		unsigned long nbytes = width * height * bytespp;
		data = new unsigned char[nbytes];
		memset(data, 0, nbytes);
	}

	TGA::TGA(const TGA& img)
	{
		width = img.width;
		height = img.height;
		bytespp = img.bytespp;
		unsigned long nbytes = width * height * bytespp;
		data = new unsigned char[nbytes];
		memcpy(data, img.data, nbytes);
	}

	TGA::~TGA()
	{
		if (data)
		{
			delete[] data;
		}
	}

	TGA& TGA::operator=(const TGA& image)
	{
		if (this != &image)
		{
			if (data)
			{
				delete[] data;
			}
			width = image.width;
			height = image.height;
			bytespp = image.bytespp;
			unsigned long nbytes = width * height * bytespp;
			data = new unsigned char[nbytes];
			memcpy(data, image.data, nbytes);
		}
		return *this;
	}

	bool TGA::read_tga_file(const char* filename)
	{
		if (data)
		{
			delete[] data;
		}
		data = NULL;
		std::ifstream in;
		in.open(filename, std::ios::binary);
		if (!in.is_open())
		{
			std::cerr << "can't open file " << filename << "\n";
			in.close();
			return false;
		}
		Header header;
		in.read((char*)&header, sizeof(header));
		if (!in.good())
		{
			in.close();
			std::cerr << "an error occured while reading the header\n";
			return false;
		}
		width = header.width;
		height = header.height;
		bytespp = header.bits_per_pixel >> 3;
		if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA))
		{
			in.close();
			std::cerr << "bad bpp (or width/height) value\n";
			return false;
		}
		unsigned long nbytes = bytespp * width * height;
		data = new unsigned char[nbytes];
		if (3 == header.data_type_code || 2 == header.data_type_code)
		{
			in.read((char*)data, nbytes);
			if (!in.good())
			{
				in.close();
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
		}
		else if (10 == header.data_type_code || 11 == header.data_type_code)
		{
			if (!load_rle_data(in))
			{
				in.close();
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
		}
		else
		{
			in.close();
			std::cerr << "unknown file format " << (int)header.data_type_code << "\n";
			return false;
		}
		if (!(header.image_descriptor & 0x20))
		{
			flip_vertically();
		}
		if (header.image_descriptor & 0x10)
		{
			flip_horizontally();
		}
		std::cerr << width << "x" << height << "/" << bytespp * 8 << "\n";
		in.close();
		return true;
	}

	bool TGA::load_rle_data(std::ifstream& in)
	{
		unsigned long pixelcount = width * height;
		unsigned long currentpixel = 0;
		unsigned long currentbyte = 0;
		Color colorbuffer;
		do
		{
			unsigned char chunkheader = 0;
			chunkheader = in.get();
			if (!in.good())
			{
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
			if (chunkheader < 128)
			{
				chunkheader++;
				for (int i = 0; i < chunkheader; i++)
				{
					in.read((char*)colorbuffer.raw, bytespp);
					if (!in.good())
					{
						std::cerr << "an error occured while reading the header\n";
						return false;
					}
					for (int t = 0; t < bytespp; t++)
					{
						data[currentbyte++] = colorbuffer.raw[t];
					}
					currentpixel++;
					if (currentpixel > pixelcount)
					{
						std::cerr << "Too many pixels read\n";
						return false;
					}
				}
			}
			else
			{
				chunkheader -= 127;
				in.read((char*)colorbuffer.raw, bytespp);
				if (!in.good())
				{
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				for (int i = 0; i < chunkheader; i++)
				{
					for (int t = 0; t < bytespp; t++)
					{
						data[currentbyte++] = colorbuffer.raw[t];
					}
					currentpixel++;
					if (currentpixel > pixelcount)
					{
						std::cerr << "Too many pixels read\n";
						return false;
					}
				}
			}
		} while (currentpixel < pixelcount);
		return true;
	}

	bool TGA::write_tga_file(const char* filename, bool rle)
	{
		unsigned char developer_area_ref[4] = { 0, 0, 0, 0 };
		unsigned char extension_area_ref[4] = { 0, 0, 0, 0 };
		unsigned char footer[18] = {
			'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O', 'N', '-', 'X', 'F', 'I', 'L', 'E', '.', '\0'
		};
		std::ofstream out;
		out.open(filename, std::ios::binary);
		if (!out.is_open())
		{
			std::cerr << "can't open file " << filename << "\n";
			out.close();
			return false;
		}
		Header header;
		memset((void*)&header, 0, sizeof(header));
		header.bits_per_pixel = bytespp << 3;
		header.width = width;
		header.height = height;
		header.data_type_code = (bytespp == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));
		header.image_descriptor = 0x20; // top-left origin
		out.write((char*)&header, sizeof(header));
		if (!out.good())
		{
			out.close();
			std::cerr << "can't dump the tga file\n";
			return false;
		}
		if (!rle)
		{
			out.write((char*)data, static_cast<std::streamsize>(width)* height* bytespp);
			if (!out.good())
			{
				std::cerr << "can't unload raw data\n";
				out.close();
				return false;
			}
		}
		else
		{
			if (!unload_rle_data(out))
			{
				out.close();
				std::cerr << "can't unload rle data\n";
				return false;
			}
		}
		out.write((char*)developer_area_ref, sizeof(developer_area_ref));
		if (!out.good())
		{
			std::cerr << "can't dump the tga file\n";
			out.close();
			return false;
		}
		out.write((char*)extension_area_ref, sizeof(extension_area_ref));
		if (!out.good())
		{
			std::cerr << "can't dump the tga file\n";
			out.close();
			return false;
		}
		out.write((char*)footer, sizeof(footer));
		if (!out.good())
		{
			std::cerr << "can't dump the tga file\n";
			out.close();
			return false;
		}
		out.close();
		return true;
	}

	// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
	bool TGA::unload_rle_data(std::ofstream& out)
	{
		const unsigned char max_chunk_length = 128;
		unsigned long npixels = width * height;
		unsigned long curpix = 0;
		while (curpix < npixels)
		{
			unsigned long chunkstart = curpix * bytespp;
			unsigned long curbyte = curpix * bytespp;
			unsigned char run_length = 1;
			bool raw = true;
			while (curpix + run_length < npixels && run_length < max_chunk_length)
			{
				bool succ_eq = true;
				for (int t = 0; succ_eq && t < bytespp; t++)
				{
					succ_eq = (data[curbyte + t] == data[curbyte + t + bytespp]);
				}
				curbyte += bytespp;
				if (1 == run_length)
				{
					raw = !succ_eq;
				}
				if (raw && succ_eq)
				{
					run_length--;
					break;
				}
				if (!raw && !succ_eq)
				{
					break;
				}
				run_length++;
			}
			curpix += run_length;
			out.put(raw ? run_length - 1 : run_length + 127);
			if (!out.good())
			{
				std::cerr << "can't dump the tga file\n";
				return false;
			}
			out.write((char*)(data + chunkstart), (raw ? run_length * bytespp : bytespp));
			if (!out.good())
			{
				std::cerr << "can't dump the tga file\n";
				return false;
			}
		}
		return true;
	}

	Color TGA::get(int x, int y)
	{
		if (!data || x < 0 || y < 0 || x >= width || y >= height)
		{
			return Color();
		}
		return Color(data + (x + y * width) * bytespp, bytespp);
	}

	bool TGA::set(int x, int y, Color c)
	{
		if (!data || x < 0 || y < 0 || x >= width || y >= height)
		{
			return false;
		}
		memcpy(data + (x + y * width) * bytespp, c.raw, bytespp);
		return true;
	}

	int TGA::get_bytespp()
	{
		return bytespp;
	}

	int TGA::get_width()
	{
		return width;
	}

	int TGA::get_height()
	{
		return height;
	}

	bool TGA::flip_horizontally()
	{
		if (!data)
		{
			return false;
		}
		int half = width >> 1;
		for (int i = 0; i < half; i++)
		{
			for (int j = 0; j < height; j++)
			{
				Color c1 = get(i, j);
				Color c2 = get(width - 1 - i, j);
				set(i, j, c2);
				set(width - 1 - i, j, c1);
			}
		}
		return true;
	}

	bool TGA::flip_vertically()
	{
		if (!data)
		{
			return false;
		}
		unsigned long bytes_per_line = width * bytespp;
		unsigned char* line = new unsigned char[bytes_per_line];
		int half = height >> 1;
		for (int j = 0; j < half; j++)
		{
			unsigned long l1 = j * bytes_per_line;
			unsigned long l2 = (height - 1 - j) * bytes_per_line;
			memmove((void*)line, (void*)(data + l1), bytes_per_line);
			memmove((void*)(data + l1), (void*)(data + l2), bytes_per_line);
			memmove((void*)(data + l2), (void*)line, bytes_per_line);
		}
		delete[] line;
		return true;
	}

	unsigned char* TGA::buffer()
	{
		return data;
	}

	void TGA::clear()
	{
		memset((void*)data, 0, width * height * bytespp);
	}

	bool TGA::scale(int w, int h)
	{
		if (w <= 0 || h <= 0 || !data)
		{
			return false;
		}
		unsigned char* tdata = new unsigned char[w * h * bytespp];
		int nscanline = 0;
		int oscanline = 0;
		int erry = 0;
		unsigned long nlinebytes = w * bytespp;
		unsigned long olinebytes = width * bytespp;
		for (int j = 0; j < height; j++)
		{
			int errx = width - w;
			int nx = -bytespp;
			int ox = -bytespp;
			for (int i = 0; i < width; i++)
			{
				ox += bytespp;
				errx += w;
				while (errx >= (int)width)
				{
					errx -= width;
					nx += bytespp;
					memcpy(tdata + nscanline + nx, data + oscanline + ox, bytespp);
				}
			}
			erry += h;
			oscanline += olinebytes;
			while (erry >= (int)height)
			{
				if (erry >= (int)height << 1) // it means we jump over a scanline
				{
					memcpy(tdata + nscanline + nlinebytes, tdata + nscanline, nlinebytes);
				}
				erry -= height;
				nscanline += nlinebytes;
			}
		}
		delete[] data;
		data = tdata;
		width = w;
		height = h;
		return true;
	}
}
