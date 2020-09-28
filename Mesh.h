#pragma once
#ifndef __MESH__
#define __MESH__
#include "Vertex.h"

class Mesh {
public:
	std::vector<Vertex> VBO;
	std::vector<int> EBO;

	Mesh CreatePlan(glm::vec3 LT, glm::vec3 LB, glm::vec3 RT, glm::vec3 RB, glm::vec4 normal)
	{
		Mesh result;
		result.VBO.push_back(Vertex(LT, glm::vec4(1, 1, 0, 1), normal, glm::vec2(0.f, 0.f)));
		result.VBO.push_back(Vertex(LB, glm::vec4(0, 1, 0, 1), normal, glm::vec2(0.f, 1.f)));
		result.VBO.push_back(Vertex(RT, glm::vec4(1, 0, 1, 1), normal, glm::vec2(1., 0.f)));
		result.VBO.push_back(Vertex(RB, glm::vec4(0, 1, 1, 1), normal, glm::vec2(1.f, 1.f)));
		//逆时针画三角形 便于之后面剔除
		result.EBO.push_back(0);
		result.EBO.push_back(1);
		result.EBO.push_back(2);

		result.EBO.push_back(1);
		result.EBO.push_back(3);
		result.EBO.push_back(2);
		return result;
	}
	void CreatCube(float ox, float oy, float oz, float radius)
	{
		//顶部
		AddMesh(CreatePlan(glm::vec3(ox - radius, oy + radius, oz - radius),
			glm::vec3(ox - radius, oy + radius, oz + radius),
			glm::vec3(ox + radius, oy + radius, oz - radius),
			glm::vec3(ox + radius, oy + radius, oz + radius),
			glm::vec4(0, -1, 0, 0)));
		//底部
		AddMesh(CreatePlan(glm::vec3(ox - radius, oy - radius, oz + radius),
			glm::vec3(ox - radius, oy - radius, oz - radius),
			glm::vec3(ox + radius, oy - radius, oz + radius),
			glm::vec3(ox + radius, oy - radius, oz - radius),
			glm::vec4(0, 1, 0, 0)));
		//右侧
		AddMesh(CreatePlan(glm::vec3(ox + radius, oy + radius, oz + radius),
			glm::vec3(ox + radius, oy - radius, oz + radius),
			glm::vec3(ox + radius, oy + radius, oz - radius),
			glm::vec3(ox + radius, oy - radius, oz - radius),
			glm::vec4(-1, 0, 0, 0)));
		//左侧
		AddMesh(CreatePlan(glm::vec3(ox - radius, oy + radius, oz - radius),
			glm::vec3(ox - radius, oy - radius, oz - radius),
			glm::vec3(ox - radius, oy + radius, oz + radius),
			glm::vec3(ox - radius, oy - radius, oz + radius),
			glm::vec4(1, 0, 0, 0)));
		//后面
		AddMesh(CreatePlan(glm::vec3(ox + radius, oy + radius, oz - radius),
			glm::vec3(ox + radius, oy - radius, oz - radius),
			glm::vec3(ox - radius, oy + radius, oz - radius),
			glm::vec3(ox - radius, oy - radius, oz - radius),
			glm::vec4(0, 0, 1, 0)));
		//前面
		AddMesh(CreatePlan(glm::vec3(ox - radius, oy + radius, oz + radius),
			glm::vec3(ox - radius, oy - radius, oz + radius),
			glm::vec3(ox + radius, oy + radius, oz + radius),
			glm::vec3(ox + radius, oy - radius, oz + radius),
			glm::vec4(0, 0, -1, 0)));
	}

	void AddMesh(Mesh Plane)
	{
		int SizeVBO = Plane.VBO.size();
		int SizeEBO = Plane.EBO.size();
		int off = VBO.size();
		for (int i = 0; i < SizeVBO; ++i)
			VBO.push_back(Plane.VBO[i]);
		for(int i = 0; i < SizeEBO; ++i)
			EBO.push_back(Plane.EBO[i]+ off);
	}
};

#endif // !__MESH__

