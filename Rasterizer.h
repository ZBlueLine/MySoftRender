#ifndef __RESTERIZER__
#define __RESTERIZER__
#include "global.h"
#include "Triangle.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Math.h"
#include "Mesh.h"

class Rasterizer {
private:
	int Width;
	int Height;
	FrameBuffer *Buf;
	Shader *shader;
	glm::mat4 ViewPortMatrix = glm::mat4(1.0f);
public:
	enum class DrawType
	{
		DrawTriangle,
		DrawLine
	};
	Rasterizer(const int &w, const int &h)
		:Width(w), Height(h)
	{
		shader = new Shader(w, h);
		Buf = new FrameBuffer(w, h);
	}
	~Rasterizer()
	{
		delete Buf;
		delete shader;
	}

	void LoadTexture(const std::string& Path)
	{
		shader->LoadTexture(Path);
	}

	void SetModelMatrix(const glm::mat4& Mat)
	{
		shader->SetModelMatrix(Mat);
	}

	void SetViewMatrix(const glm::mat4& Mat)
	{
		shader->SetViewMatrix(Mat);
	}

	void SetPerspectiveMatrix(const glm::mat4& Mat)
	{
		shader->SetPerspectiveMatrix(Mat);
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

	void PerspectiveDivision(VtoR &p)
	{
		p.windowp /= p.windowp.w;
		p.windowp.w = 1.0f;

		p.windowp.z = (p.windowp.z + 1.0) * 0.5f;
	}

	bool BackFaceculling(const VtoR& p1, const VtoR& p2, const VtoR& p3)
	{
		glm::vec3 v1 = glm::vec3(p2.windowp.x - p1.windowp.x, p2.windowp.y - p1.windowp.y, p2.windowp.z - p1.windowp.z);
		glm::vec3 v2 = glm::vec3(p3.windowp.x - p1.windowp.x, p3.windowp.y - p1.windowp.y, p3.windowp.z - p1.windowp.z);
		//ndc空间是左手系，交换一下叉乘顺序
		v2 = glm::cross(v2, v1);
		glm::vec3 eyev(0, 0, 1);
		return glm::dot(eyev, v2) < 0;
	}

	void DrawMesh(const Mesh& mesh, const DrawType type)
	{
		int Size = mesh.EBO.size();
		for (int i = 0; i < Size; i += 3)
		{
			DrawTriangle(Triangle(mesh.VBO[mesh.EBO[i]], mesh.VBO[mesh.EBO[i + 1]], mesh.VBO[mesh.EBO[i + 2]]), type);
		}

	}


	void DrawTriangle(const Triangle& triangle, const DrawType type)
	{
		Vertex v1 = triangle.v1;
		Vertex v2 = triangle.v2;
		Vertex v3 = triangle.v3;
		VtoR p1 = shader->VertexShader(v1);
		VtoR p2 = shader->VertexShader(v2);
		VtoR p3 = shader->VertexShader(v3);

		PerspectiveDivision(p1);
		PerspectiveDivision(p2);
		PerspectiveDivision(p3);
		if (!BackFaceculling(p1, p2, p3))
			return;
		p1.windowp = ViewPortMatrix * p1.windowp;
		p2.windowp = ViewPortMatrix * p2.windowp;
		p3.windowp = ViewPortMatrix * p3.windowp;

		if (type == DrawType::DrawLine)
		{
			Drawline(p1, p2);
			Drawline(p2, p3);
			Drawline(p1, p3);
		}
		else 
		{
			ScanTriangle(p1, p2, p3);
		}
	}

private:
	void ScanTriangle(VtoR v1, VtoR v2, VtoR v3)
	{
		//确保v1 v2 v3是y从大到小
		if (v1.windowp.y < v2.windowp.y)
			std::swap(v1, v2);
		if(v2.windowp.y < v3.windowp.y)
			std::swap(v2, v3);
		if (v1.windowp.y < v2.windowp.y)
			std::swap(v1, v2);

		if (equal(v1.windowp.y, v2.windowp.y))
			DownTriangle(v1, v2, v3);//这是一个底三角形
		else if (equal(v2.windowp.y, v3.windowp.y))
			UpTriangle(v1, v2, v3);
		else
		{
			float Weight = (v1.windowp.y - v2.windowp.y)/(v1.windowp.y - v3.windowp.y);
			VtoR tmpv = VtoR::Lerp(v1, v3, Weight);
			tmpv.windowp.y = v2.windowp.y;
			UpTriangle(v1, v2, tmpv);
			DownTriangle(v2, tmpv, v3);
		}
	}
	void UpTriangle(const VtoR& v1, const VtoR& v2, const VtoR& v3)
	{
		float Starty = v1.windowp.y;
		float Range = v1.windowp.y - v3.windowp.y;
		int Beginy = v1.windowp.y;

		for (int i = 0; i <= Range; ++i)
		{
			float Weight = i / Range;
			VtoR Nleft = VtoR::Lerp(v1, v2, Weight);
			VtoR Nright = VtoR::Lerp(v1, v3, Weight);
			Nleft.windowp.x = (int)Nleft.windowp.x;
			Nright.windowp.x = (int)(Nright.windowp.x);

			Nright.windowp.y = Nleft.windowp.y = Beginy;
			--Beginy;
			Scanline(Nleft, Nright);
		}
	}

	void DownTriangle(const VtoR& v1, const VtoR& v2, const VtoR& v3)
	{
		float Starty = v1.windowp.y;
		float Range = v1.windowp.y - v3.windowp.y;
		int Beginy = v1.windowp.y;

		for (int i = 0; i <= Range; ++i)
		{
			float Weight = i / Range;
			VtoR Nleft = VtoR::Lerp(v2, v3, Weight);
			VtoR Nright = VtoR::Lerp(v1, v3, Weight);
			Nleft.windowp.x = (int)Nleft.windowp.x;
			Nright.windowp.x = (int)(Nright.windowp.x);

			Nright.windowp.y = Nleft.windowp.y = Beginy;
			--Beginy;
			Scanline(Nleft, Nright);
		}
	}
	void Scanline(VtoR left, VtoR right)
	{
		if (left.windowp.x > right.windowp.x)
			std::swap(left, right);
		float Range = right.windowp.x - left.windowp.x;
		for (int i = 0; i <= Range; ++i)
		{
			float Weight = i / Range;
			VtoR New = VtoR::Lerp(left, right, Weight);
			New.windowp.y = left.windowp.y;
			New.windowp.x = left.windowp.x + i;

			New.worldp /= New.Z;
			New.color /= New.Z;
			New.normal /= New.Z;
			New.texcoord /= New.Z;

			if (New.windowp.z < Buf->GetDepth(New.windowp.x, New.windowp.y))
			{
				Buf->WriteDepth(New.windowp.x, New.windowp.y, New.windowp.z);
				/*std::cout << shader.FragmentShader(New).x << ' ' << shader.FragmentShader(New).y << ' ' << shader.FragmentShader(New).z << std::endl;*/
				Buf->Setpoint(New.windowp.x, New.windowp.y, shader->FragmentShader(New));
			}
		}
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