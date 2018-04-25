//
//  wavefront_obj.cpp
//  MyRenderer
//

#include "wavefront_obj.hpp"
#include <sstream>
#include "../shape/line.hpp"

using geometry::Vec2i;
using geometry::Vec2f;
using geometry::Vec3;
using geometry::Vec3f;

namespace model
{
	WavefrontObj::WavefrontObj(std::string path) : vertices(), faces()
	{
		std::ifstream in;
		in.open(path, std::ifstream::in);
		if (in.fail())
		{
			return;
		}
		std::string line;
		while (!in.eof())
		{
			std::getline(in, line);
			std::istringstream iss(line.c_str());
			char trash;
			if (!line.compare(0, 2, "v "))
			{
				Vec3f vertex;
				iss >> trash >> vertex;
				vertices.push_back(vertex);
			}
			else if (!line.compare(0, 2, "f "))
			{
				std::vector<Vec3<std::size_t>> f;
				unsigned int index;
				unsigned int itrash;
				iss >> trash;
				while (iss >> index >> trash >> itrash >> trash >> itrash)
				{
					--index; // in wavefront obj all indices start at 1, not zero
					f.push_back(Vec3<std::size_t>(index, 0, 0));
				}
				faces.push_back(f);
			}
			else
			{
				// TODO
			}
		}
		std::cerr << "# v# " << vertices.size() << " f# "  << faces.size() << std::endl;
	}

	WavefrontObj::~WavefrontObj() {}

	std::size_t WavefrontObj::vertice_number() const
	{
		return vertices.size();
	}

	std::size_t WavefrontObj::face_number() const
	{
		return faces.size();
	}

	Vec3f WavefrontObj::vertex(std::size_t nth_vertex) const
	{
		return vertices[nth_vertex];
	}

	std::vector<std::size_t> WavefrontObj::face(std::size_t nth_face) const
	{
		std::vector<std::size_t> face;
		for (std::size_t i = 0; i < faces[nth_face].size(); ++i)
		{
			face.push_back(faces[nth_face][i].x);
		}
		return face;
	}

	void WavefrontObj::draw(image::TGA &image, const image::Color& color) const
	{
		int width = image.get_width();
		int height = image.get_height();
		for (std::size_t i = 0; i < face_number(); ++i)
		{
			std::vector<std::size_t> face = this->face(i);
			std::size_t vertex_number = face.size();
			for (std::size_t j = 0; j < vertex_number; ++j)
			{
				Vec3f v0 = vertex(face[j]);
				Vec3f v1 = vertex(face[(j + 1) % vertex_number]);
				Vec2i p0{static_cast<int>((v0.x + 1.f) * width / 2.f), static_cast<int>((v0.y + 1.f) * height / 2.f)};
				Vec2i p1{static_cast<int>((v1.x + 1.f) * width / 2.f), static_cast<int>((v1.y + 1.f) * height / 2.f)};
				shape::line(p0, p1, image, color);
			}
		}
	}
}
