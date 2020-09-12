#pragma once
#ifndef __MATH__
#define __MATH__
#include "global.h"

glm::mat4 GetModelMatrix()
{
	return glm::mat4(1.0f);
}

glm::mat4 GetViewMatrix()
{
	return glm::mat4(1.0f);
}

glm::mat4 GetProjectionMatrix()
{
	return glm::mat4(1.0f);
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

glm::vec4 lerp(const glm::vec4& v1, const glm::vec4& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}
glm::vec3 lerp(const glm::vec3& v1, const glm::vec3& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}
glm::vec2 lerp(const glm::vec2& v1, const glm::vec2& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}


bool equal(float a, float b)
{
	float esp = 1e-8;
	return fabs(a - b) < esp ? true : false;
}

#endif // !__MATH__

