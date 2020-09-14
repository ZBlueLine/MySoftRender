#pragma once
#ifndef __MATH__
#define __MATH__
#include "global.h"

glm::mat4 GetModelMatrix()
{
	return glm::mat4(1.0f);
}



glm::mat4 GetViewMatrix(glm::vec4 Position, glm::vec3 Front, glm::vec3 Up)
{
	glm::mat4 result;
	glm::vec3 UcF = cross(Front, Up);
	glm::mat4 tranform(1.f, 0, 0, 0,
		0, 1.f, 0, 0,
		0, 0, 1.f, 0,
		-Position.x, -Position.y, -Position.z, 1);
	glm::mat4 Rotation(UcF.x, Up.x, -Front.x, 0,
		UcF.y, Up.y, -Front.y, 0,
		UcF.z, Up.z, -Front.z, 0,
		0, 0, 0, 1);
	result = Rotation * tranform;
	return result;
}


glm::mat4 GetPerspectiveMatrix(const float& fov, const float& AspectRatio, const float& Near, const float& Far)
{
	//进行投影变换的时候摄像机已经在原点了
	glm::mat4 result;
	float n = Near;
	float f = Far;
	float t = tan(glm::radians(fov*0.5f)) * n;
	float b = -t;
	float r = AspectRatio * t;
	float l = -r;
	//这是构成整个投影矩阵的三个矩阵
	/*glm::mat4 pre(n, 0, 0, 0,
		0, n, 0, 0,
		0, 0, n + f, 1,
		0, 0, -n * f, 0);

	glm::mat4 ort(2 / (r - l), 0, 0, 0,
				0, 2 / (t - b), 0, 0,
				0, 0, 2 / (n - f), 0,
				0, 0, 0, 1);

	glm::mat4 tmp(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				-(r + l) * 0.5, -(t + b) * 0.5, -(n + f) * 0.5, 1);
	result = ort* tmp * pre;
	result[2][3] *= -1;*/
	//可以化简为如下矩阵
	result = glm::mat4(n / r, 0, 0, 0,
		0, n / t, 0, 0,
		0, 0, -(f + n) / (f - n), -1,
		0, 0, -2 * f * n / (f - n), 0);
	//for (int i = 0; i < 4; ++i)
	//{
	//	for (int j = 0; j < 4; ++j)
	//	{
	//		std::cout << result[i][j] << ' ';
	//	}
	//	std::cout << std::endl;
	//}
	return result;
}




glm::mat4 GetViewPortMatrix(float Width, float Height)
{
	glm::mat4 result(1.0f);
	//先缩放再平移，所以只要拼在一起
	result[0][0] = Width / 2;
	result[1][1] = Height / 2;
	result[3][0] = Width / 2;
	result[3][1] = Height / 2;
	return result;
}

glm::mat4 RodriguesRotationFormula(glm::vec3 point, glm::vec3 axis, float angle)
{
	glm::mat3 rotation = glm::mat3(1.0f);
	glm::mat4 ans, trans;
	glm::mat3 I = glm::mat3(1.0f);
	glm::mat3 cross_product;
	cross_product = glm::mat3(0, axis[2], -axis[1],
		-axis[2], 0, axis[0],
		axis[1], -axis[0], 0);
	glm::mat3 TAxis(axis.x, 0, 0,
		axis.y, 0, 0,
		axis.z, 0, 0);
	glm::mat3 Axis(axis.x, axis.y, axis.z,
		0, 0, 0,
		0, 0, 0);
	rotation = cos(glm::radians(angle)) * I + (1 - cos(glm::radians(angle))) * Axis * TAxis + sin(glm::radians(angle)) * cross_product;
	trans = glm::mat4(1, 0, 0, point[0],
		0, 1, 0, point[1],
		0, 0, 1, point[2],
		0, 0, 0, 1);
	ans = glm::mat4(glm::vec4(rotation[0], 0),
		glm::vec4(rotation[1], 0),
		glm::vec4(rotation[2], 0),
		glm::vec4(0, 0, 0, 1));
	ans = trans * ans * glm::inverse(trans);
	return ans;
}



glm::vec4 lerp(const glm::vec4& v1, const glm::vec4& v2, float factor) {
	return factor*(v2 - v1) + v1;
}
glm::vec3 lerp(const glm::vec3& v1, const glm::vec3& v2, float factor) {
	return factor * (v2 - v1) + v1;
}
glm::vec2 lerp(const glm::vec2& v1, const glm::vec2& v2, float factor) {
	return factor * (v2 - v1) + v1;
}
float lerp(const float& v1, const float& v2, float factor)
{
	return factor*(v2 - v1) + v1;
}


bool equal(float a, float b)
{
	float esp = 1e-8;
	return fabs(a - b) < esp ? true : false;
}

#endif // !__MATH__

