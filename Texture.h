#pragma once
#ifndef __TEXTURE__
#define __TEXTURE__

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class Texture {
protected:
	int Width, Height;
	int channels = 4;
private:
	unsigned char* data;
public:
	Texture(int wid = 0, int hei = 0)
	{
		data = nullptr;
		Width = wid;
		Height = hei;
	}
	Texture(const Texture& other)
	{
		data = new unsigned char[sizeof(other.data)];
		*data = *other.data;
		Width = other.Width;
		Height = other.Height;
	}
	virtual ~Texture()
	{
		if(data)
			delete[] data;
	}
	virtual bool isempty()
	{
		if (data)
			return false;
		return true;
	}

	virtual void LoadTexture(const std::string &Path)
	{
		if (data)
			delete[] data;
		data = stbi_load(Path.c_str(), &Width, &Height, &channels, 0);
	}

	virtual glm::vec4 GetColor(const glm::vec2 tex)
	{
		//超出部分直接从头开始取
		float fx = tex.x - (float)floor(tex.x);
		float fy = tex.y - (float)floor(tex.y);
		fx = fx < 0 ? -fx : fx;
		fy = fy < 0 ? -fy : fy;
		//std::cout << tex.x << ' ' << tex.y << std::endl;
		int x = fx* Width;
		int y = fy * Height;
		if (x < 0 || x > Width || y < 0 || y>Height)
			return glm::vec4(0, 0, 0, 0);
		int i = channels * (y * Width + x);
		return glm::vec4(*(data + i), *(data + i + 1), *(data + i + 2), *(data + i + 3))/255.f;
	}
};

#endif // !__TEXTURE__
