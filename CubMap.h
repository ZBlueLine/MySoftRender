#pragma once
#ifndef __CUBEMAP__
#define __CUBEMAP__

#include <iostream>
#include "Texture.h"


class CubeMap: public Texture 
{
	public:
		unsigned char* data;
	~CubeMap()
	{
		if (data)
			delete[] data;
	}

	void LoadTexture(const std::string& Path)
	{
		if (data)
			delete[] data;
		data = stbi_load(Path.c_str(), &Width, &Height, &channels, 0);
		
	}

	glm::vec4 GetColor(const glm::vec3 tex)
	{
		//超出部分直接从头开始取
		float fx = tex.x - (float)floor(tex.x);
		float fy = tex.y - (float)floor(tex.y);
		fx = fx < 0 ? -fx : fx;
		fy = fy < 0 ? -fy : fy;
		//std::cout << tex.x << ' ' << tex.y << std::endl;
		int x = fx * Width;
		int y = fy * Height;
		if (x < 0 || x > Width || y < 0 || y>Height)
			return glm::vec4(0, 0, 0, 0);
		int i = channels * (y * Width + x);
		return glm::vec4(*(data + i), *(data + i + 1), *(data + i + 2), *(data + i + 3)) / 255.f;
	}
};

#endif // !__CUBEMAP__