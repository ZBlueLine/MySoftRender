#pragma once
#ifndef __FRAGMENTBUFFER__
#define __FRAGMENTBUFFER__
#include <vector>


class FrameBuffer {
private:
	int Height;
	int Width;
	//���ڴ洢ͼ�񻺳壬ɫ����4ͨ����rgba
public:
	std::vector<unsigned char> ColorData;
	FrameBuffer(){}
	FrameBuffer(const int &w, const int &h)
		: Width(w), Height(h)
	{
		ColorData.resize(Width * Height * 4);
	}
	~FrameBuffer() = default;
	
	//������ջ���
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

	//void Setpoint(const int &x, const int &y, const glm::vec4 color)
	//{
	//	if (x < 0 || x >= Width || y < 0 || y >= Height)
	//		return;
	//	int i = y * Width + x;
	//	ColorData[i * 4] = color.r;
	//	ColorData[i * 4 + 1] = color.g;
	//	ColorData[i * 4 + 2] = color.b;
	//	ColorData[i * 4 + 3] = color.a;
	//}
	void Setpoint(const int& x, const int& y, const glm::vec4& color) {
		if (x < 0 || x >= Width || y < 0 || y >= Height)
			return;
		int xy = (y * Width + x);
		ColorData[xy * 4] = color.r;
		ColorData[xy * 4 + 1] = color.g;
		ColorData[xy * 4 + 2] = color.b;
		ColorData[xy * 4 + 3] = color.a;
	}


};



#endif // !__FRAGMENTBUFFER__

