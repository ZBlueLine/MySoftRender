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

	//std::vector<glm::vec3> ViewPortPoint
	//{ 
	//	//右上左上左下右下四个点
	//	//逆时针方向，用于判断内外
	//	glm::vec3(1.f, 1.f, 1.f),
	//	glm::vec3(-1.f, 1.f, 1.f),
	//	glm::vec3(-1.f, -1.f, 1.f), 
	//	glm::vec3(1.f, -1.f, 1.f) 
	//};

	//bool InsideLine(const glm::vec3& p1, const glm::vec3 &p2, const glm::vec4 &Point)
	//{
	//	glm::vec3 point = glm::vec3(Point.x, Point.y, Point.z);
	//	//从p1到p2的向量
	//	glm::vec3 line1 = p2 - p1;
	//	glm::vec3 line2 = point - p1;

	//	return glm::normalize(cross(line1, line2)).z > 0;
	//}

	//bool Allinside(const std::vector<VtoR> &points)
	//{
	//	for (const VtoR &c:points)
	//	{
	//		for (int j = 0; j < 4; ++j)
	//		{
	//			if (!InsideLine(ViewPortPoint[j], ViewPortPoint[(j + 1)%4], c.windowp))
	//				return false;
	//		}
	//	}
	//	return true;
	//}

	//VtoR Intersect(const glm::vec3& lp1, const glm::vec3& lp2, const VtoR& p1, const VtoR& p2)
	//{
	//	float Weight = 0.f;
	//	if (lp1.y == lp2.y)
	//		Weight = fabs(p1.windowp.y - lp1.y) / fabs(p1.windowp.y - p2.windowp.y);
	//	else
	//		Weight = fabs(p1.windowp.x - lp1.x) / fabs(p1.windowp.x - p2.windowp.x);
	//	return VtoR::Lerp(p1, p2, Weight);
	//}

	std::vector<glm::vec4> ViewPortPlane
	{
		glm::vec4(0, 1.f, 0, 1.f),
		glm::vec4(0, -1.f, 0, 1.f),

		glm::vec4(0, 0, 1.f, 1.f),
		glm::vec4(0, 0, -1.f, 1.f),

		glm::vec4(1.f, 0, 0, 1.f),
		glm::vec4(-1.f, 0, 0, 1.f)
	};
	
	bool InsidePlane(const glm::vec4& plane, const glm::vec4& Point)
	{
		if (plane.x * Point.x + plane.y * Point.y + plane.z * Point.z + plane.w * Point.w > 0)
			return true;
		return false;
	}

	bool Allinside(const std::vector<VtoR> &points)
	{
		for (const VtoR &c:points)
		{
			for (int j = 0; j < 6; ++j)
			{
				if (!InsidePlane(ViewPortPlane[j], c.windowp))
					return false;
			}
		}
		return true;
	}

	VtoR Intersect(const glm::vec4& Plane, const VtoR& p1, const VtoR& p2)
	{
		//这里的windowp都还没有做透视除法，所以按照公式Ax/w + By/w + Cz/w + D需要乘w；
		float d1 = Plane.x * p1.windowp.x + Plane.y * p1.windowp.y + Plane.z * p1.windowp.z + Plane.w * p1.windowp.w;
		float d2 = Plane.x * p2.windowp.x + Plane.y * p2.windowp.y + Plane.z * p2.windowp.z + Plane.w * p2.windowp.w;
		//p1 p2在面两侧，所以符号相反，就需要加一个减号求和
		float Weight = d1 / (d1 - d2);
		return VtoR::Lerp(p1, p2, Weight);
	}

	std::vector<VtoR> ViewClip(const VtoR& v1, const VtoR& v2, const VtoR& v3)
	{
		std::vector<VtoR> output{ v1, v2, v3};
		if (Allinside(output))
			return output;

		for (int e = 0; e < 6; ++e)
		{
			std::vector<VtoR> input(output);
			output.clear();
			int Sizeofi = input.size();
			for (int i = 0; i < Sizeofi; ++i)
			{
				VtoR current = input[i];
				VtoR prev = input[(i + Sizeofi - 1) % Sizeofi];
				if (InsidePlane(ViewPortPlane[e], current.windowp))
				{
					if (!InsidePlane(ViewPortPlane[e], prev.windowp))
						output.push_back(Intersect(ViewPortPlane[e], current, prev));
					output.push_back(current);
				}
				else if (InsidePlane(ViewPortPlane[e], prev.windowp))
					output.push_back(Intersect(ViewPortPlane[e], current, prev));
			}
		}

		return output;
	}

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
		//对于一个在摄像机之后的物体，w会是一个负值，这就导致上下左右的颠倒
		//而且目前没有做齐次坐标剔除，在背后的东西也会被画出来，因为画一个物体只取决于深度测试
		//位于背后的物体深度是很浅的，就直接通过xy画了
		p.Z = 1 / p.windowp.w;
		p.windowp /= p.windowp.w;
		p.windowp.w = 1.0f;

		p.windowp.z = (p.windowp.z + 1.0) * 0.5f;
		p.worldp *= p.Z;
		p.color *= p.Z;
		p.texcoord *= p.Z;
		p.normal *= p.Z;
	}

	bool Homogeneousculling(const VtoR& p1, const VtoR& p2, const VtoR& p3)
	{
		if (p1.windowp.x < -p1.windowp.w && p2.windowp.x < -p2.windowp.w && p3.windowp.x < -p3.windowp.w)
			return false;
		if (p1.windowp.x > p1.windowp.w && p2.windowp.x > p2.windowp.w && p3.windowp.x > p3.windowp.w)
			return false;
		if (p1.windowp.y < -p1.windowp.w && p2.windowp.y < -p2.windowp.w && p3.windowp.y < -p3.windowp.w)
			return false;
		if (p1.windowp.y > p1.windowp.w && p2.windowp.y > p2.windowp.w && p3.windowp.y > p3.windowp.w)
			return false;
		if (p1.windowp.z < -p1.windowp.w && p2.windowp.z < -p2.windowp.w && p3.windowp.z < -p3.windowp.w)
			return false;
		if (p1.windowp.z > p1.windowp.w && p2.windowp.z > p2.windowp.w && p3.windowp.z > p3.windowp.w)
			return false;
		return true;
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

		std::vector<VtoR> Npoints = ViewClip(p1, p2, p3);
		int Trianglenum = Npoints.size() - 3 + 1;
		for (int i = 0; i < Trianglenum; ++i)
		{
			p1 = Npoints[0];
			p2 = Npoints[i + 1];
			p3 = Npoints[i + 2];
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
		glm::vec4 line_color = { 1, 1, 1, 1 };
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