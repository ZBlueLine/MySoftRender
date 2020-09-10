#pragma once
#ifndef __FRAGMENTBUFFER__
#define __FRAGMENTBUFFER__
#include <vector>


class FrameBuffer {
private:
	int Height;
	int Width;
	//用于存储图像缓冲，色彩用4通道的rgba
public:
	std::vector<unsigned char> ColorData;
	FrameBuffer(const int &w, const int &h)
		: Width(w), Height(h)
	{
		ColorData.resize(Width * Height * 4);
	}
	~FrameBuffer() = default;
	
	//用于清空画布
	void ClearBuffer(glm::vec4 color)
	{
		unsigned char *p = ColorData.data();
		for (int i = 0; i < Height * Width * 4; i += 4)
		{
			p[i] = color.r;
			p[i + 1] = color.g;
			p[i + 2] = color.b;
			p[i + 3] = color.a;
		}
	}
};



#endif // !__FRAGMENTBUFFER__

