#pragma once
#ifndef __TEXTURECUBE__
#define __TEXTURECUBE__

#include <iostream>
#include "Texture.h"
#include <algorithm>

class TextureCUBE : public Texture{
private:
	unsigned char* Front;
	unsigned char* Back;

	unsigned char* Top;
	unsigned char* Bottom;

	unsigned char* Left;
	unsigned char* Right;
	unsigned char* CubeMap[6];
public:
	TextureCUBE(int wid = 0, int hei = 0)
	{
		for (int i = 0; i < 6; ++i)
			CubeMap[i] = nullptr;
		Front = nullptr;
		Back = nullptr;

		Top = nullptr;
		Bottom = nullptr;

		Left = nullptr;
		Right = nullptr;

		Width = wid;
		Height = hei;
	}
	TextureCUBE(const TextureCUBE& other)
	{
		Front = new unsigned char[sizeof(other.Front)];
		*Front = *other.Front;
		Width = other.Width;
		Height = other.Height;
	}
	virtual ~TextureCUBE()
	{
		for (int i = 0; i < 6; ++i)
		{
			if(CubeMap[i] != nullptr)
				CubeMap[i] = nullptr;
		}
		if (Front)
			delete[] Front;
		if (Back)
			delete[] Back;

		if (Top)
			delete[] Top;
		if (Bottom)
			delete[] Bottom;

		if (Left)
			delete[] Left;
		if (Right)
			delete[] Right;
	}
	virtual bool isempty() const
	{
		for (int i = 0; i < 6; ++i)
			if (CubeMap[i] == nullptr)
				return false;
		if (Front|| Back || Top || Bottom|| Left|| Right)
			return false;
		return true;
	}
	//Front Back Top Botton Left Right
	virtual void LoadTexture(const std::vector<std::string>& CubeMapPath)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (CubeMap[i] != nullptr)
				CubeMap[i] = nullptr;
		}
		if (Front)
			delete[] Front;
		if (Back)
			delete[] Back;

		if (Top)
			delete[] Top;
		if (Bottom)
			delete[] Bottom;

		if (Left)
			delete[] Left;
		if (Right)
			delete[] Right;
		for(int i = 0; i < 6; ++i)
			CubeMap[i] = stbi_load(CubeMapPath[i].c_str(), &Width, &Height, &channels, 0);
		Front = stbi_load(CubeMapPath[0].c_str(), &Width, &Height, &channels, 0);
		Back = stbi_load(CubeMapPath[0].c_str(), &Width, &Height, &channels, 0);

		Top = stbi_load(CubeMapPath[0].c_str(), &Width, &Height, &channels, 0);
		Bottom = stbi_load(CubeMapPath[0].c_str(), &Width, &Height, &channels, 0);

		Left = stbi_load(CubeMapPath[0].c_str(), &Width, &Height, &channels, 0);
		Right = stbi_load(CubeMapPath[0].c_str(), &Width, &Height, &channels, 0);
	}

	glm::vec4 GetColorByIndex(const glm::vec2 tex, const int index) const
	{
		//超出部分直接从头开始取
		float fx = tex.x - (float)floor(tex.x);
		float fy = tex.y - (float)floor(tex.y);
		fx = fx < 0 ? -fx : fx;
		fy = fy < 0 ? -fy : fy;
		int x = fx * Width;
		int y = fy * Height;
		if (x < 0 || x > Width || y < 0 || y>Height)
			return glm::vec4(0, 0, 0, 0);
		int i = channels * (y * Width + x);
		return glm::vec4(*(CubeMap[index] + i), *(CubeMap[index] + i + 1), *(CubeMap[index] + i + 2), *(CubeMap[index] + i + 3)) / 255.f;
	}

	virtual glm::vec4 GetColor(const glm::vec2& tex, float texcoordz = 0) const
	{
		glm::vec3 dir = glm::vec3(tex.x, tex.y, texcoordz);
		float x = std::fabs(dir.x);
		float y = std::fabs(dir.y);
		float z = std::fabs(dir.z);
		int index = 0;
		glm::vec2 texcoord;
		if (x > y && x > z)
		{
			index = dir.x < 0 ? 4 : 5;
			if(dir.x < 0)
				texcoord = glm::vec2(dir.z / x + 1, -dir.y / x + 1) * 0.5f;
			else
				texcoord = glm::vec2(-dir.z / x + 1, -dir.y / x + 1) * 0.5f;
		}
		else if (y > x && y > z)
		{
			index = dir.y < 0 ? 3 : 2;
			if (dir.y < 0)
				texcoord = glm::vec2(dir.x / y + 1, -dir.z / y + 1) * 0.5f;
			else
				texcoord = glm::vec2(dir.x / y + 1, dir.z / y + 1) * 0.5f;
		}
		else if (z > x && z > y)
		{
			index = dir.z < 0 ? 1 : 0;
			if (dir.z < 0)
				texcoord = glm::vec2(-dir.x / z + 1, -dir.y / z + 1) * 0.5f;
			else
				texcoord = glm::vec2(dir.x / z + 1, -dir.y / z + 1) * 0.5f;
		}
		//return glm::vec4(texcoord.x, texcoord.y, 0, 1);
		return GetColorByIndex(texcoord, index);
	}
};

#endif // !__TEXTURE__
