#ifndef __RESTERIZER__
#define __RESTERIZER__
#include "global.h"
#include "Triangle.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Math.h"

class Rasterizer {
private:
	int Width;
	int Height;
	FrameBuffer *Buf;
	Shader shader;
	glm::mat4 ViewPortMatrix = glm::mat4(1.0f);
public:
	enum class DrawType
	{
		DrawTriangle,
		DrawLine
	};

	Rasterizer(){}
	Rasterizer(const int &w, const int &h)
		:Width(w), Height(h)
	{
		Buf = new FrameBuffer(w, h);
	}

	void SetModelMatrix(const glm::mat4& Mat)
	{
		shader.SetModelMatrix(Mat);
	}

	void SetViewMatrix(const glm::mat4& Mat)
	{
		shader.SetModelMatrix(Mat);
	}

	void SetProjectionMatrix(const glm::mat4& Mat)
	{
		shader.SetModelMatrix(Mat);
	}

	void SetViewPortMatrix(const glm::mat4& Mat)
	{
		ViewPortMatrix = Mat;
	}

	void ClearBuffer(glm::vec4 color)
	{
		Buf->ClearBuffer(color);
	}

	void Show()
	{
		glDrawPixels(Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, Buf->ColorData.data());
	}

	void DrawTriangle(Triangle& triangle, DrawType type)
	{
		Vertex v1 = triangle.v1;
		Vertex v2 = triangle.v2;
		Vertex v3 = triangle.v3;

		VtoR p1 = shader.VertexShader(v1);
		VtoR p2 = shader.VertexShader(v2);
		VtoR p3 = shader.VertexShader(v3);

		p1.windowp = ViewPortMatrix * p1.windowp;
		p2.windowp = ViewPortMatrix * p2.windowp;
		p3.windowp = ViewPortMatrix * p3.windowp;

		if (type == DrawType::DrawLine)
		{
			Drawline(p1, p2);
			Drawline(p2, p3);
			Drawline(p1, p3);
		}
		//std::cout << p1.windowp.x << ' ' << p1.windowp.y << ' ' << p1.windowp.z << std::endl;
		//std::cout << p2.windowp.x << ' ' << p2.windowp.y << ' ' << p2.windowp.z << std::endl;
		//std::cout << p3.windowp.x << ' ' << p3.windowp.y << ' ' << p3.windowp.z << std::endl;


	}

	void ScanTriangle(VtoR v1, VtoR v2, VtoR v3)
	{
		//确保v1 v2 v3是y从大到小
		if (v1.windowp.y < v2.windowp.y)
			std::swap(v1, v2);
		if(v2.windowp.y < v3.windowp.y)
			std::swap(v2, v3);
		if (v1.windowp.y < v3.windowp.y)
			std::swap(v1, v3);
		if (equal(v1.windowp.y, v2.windowp.y))
			DownTriangle(v1, v2, v3);//这是一个底三角形
		else if (equal(v2.windowp.y, v3.windowp.y))
			UpTriangle(v1, v2, v3);
		else
		{
			float Weight = (v1.windowp.y - v2.windowp.y)/(v1.windowp.y - v3.windowp.y);
			VtoR tmpv = VtoR::Lerp(v1, v3, Weight);
			UpTriangle(v1, v2, tmpv);
			DownTriangle(v2, v3, tmpv);
		}
	}
private:
	void UpTriangle(VtoR v1, VtoR v2, VtoR v3)
	{
		float Starty = v1.windowp.y;
		float Range = v1.windowp.y - v3.windowp.y;
		for (int i = 0; i < Range; ++i)
		{

		}
	}

	void DownTriangle(VtoR v1, VtoR v2, VtoR v3)
	{

	}
public:
	void Drawline(VtoR begin, VtoR end)
	{
		float x0 = begin.windowp.x;
		float y0 = begin.windowp.y;
		float x1 = end.windowp.x;
		float y1 = end.windowp.y;
		glm::vec4 line_color = { 255, 255, 255, 255 };
		//处理斜率大于1的情况
		bool steep = abs(y1 - y0) > abs(x1 - x0);
		if (steep)
		{
			std::swap(x1, y1);
			std::swap(x0, y0);
		}
		//处理从右向左画的情况
		if (x0 > x1)
		{
			std::swap(x1, x0);
			std::swap(y1, y0);
		}
		int delatex = x1 - x0;
		int delatey = abs(y1 - y0);
		int error = delatex / 2;
		int stepy;
		//斜率为负的情况
		y1 < y0 ? stepy = -1 : stepy = 1;
		int y = y0;
		for (int x = x0; x <= x1; ++x)
		{
			if (steep)
			{
				glm::vec3 point = glm::vec3(y, x, 1.0f);
				Buf->Setpoint(point.x, point.y, line_color);
			}
			else
			{
				glm::vec3 point = glm::vec3(x, y, 1.0f);
				Buf->Setpoint(point.x, point.y, line_color);
			}
			error -= delatey;
			if (error < 0)
			{
				y += stepy;
				error += delatex;
			}
		}
	}
};

#endif // !__RESTERIZER__