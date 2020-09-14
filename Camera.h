#pragma once
#ifndef __CAMERA__
#define __CAMERA__
#include <glm/glm.hpp>
#include "Math.h"


class Camera {
private:
	float fov;
	glm::vec4 Position;
	glm::vec4 UpV;
	glm::vec4 RightV;
	glm::vec4 FrontV;
public:
	Camera(
		const glm::vec3& _pos = glm::vec3(0, 0, 5),
		const glm::vec3& _fv = glm::vec3(0, 0, -1),
		const glm::vec3& _rv = glm::vec3(1, 0, 0),
		const glm::vec3& _upv = glm::vec3(0, 1, 0),
		const float& _fov = 60.f
	)
		:Position(_pos, 1.0f), FrontV(_fv, 0), RightV(_rv, 0), UpV(_upv, 0), fov(_fov)
	{}

	void MoveZ(const float& step)
	{
		Position += step* FrontV;
	}

	void MoveX(const float& step)
	{
		Position += step* RightV;
	}

	void MoveY(const float& step)
	{
		Position += step* UpV;
	}


	//使用旋转矩阵，都是逆时针旋转
	void pitch(const float& angle)
	{
		float radian = glm::radians(angle);
		glm::mat4 Rx = RodriguesRotationFormula(Position, RightV, angle);
		FrontV = Rx * FrontV;
		UpV = Rx * UpV;
	}

	void yaw(const float& angle)
	{
		float radian = glm::radians(angle);
		glm::mat4 Ry = RodriguesRotationFormula(Position, glm::vec3(0, 1, 0), angle);
		UpV = Ry * UpV;
		FrontV = Ry * FrontV;
		RightV = Ry * RightV;
	}

	float GetFov() const
	{
		return fov;
	}

	glm::vec4 GetPosition() const
	{
		return Position;
	}

	glm::vec4 GetUp() const
	{
		return UpV;
	}

	glm::vec4 GetRight() const
	{
		return RightV;
	}

	glm::vec4 GetFront() const
	{
		return FrontV;
	}
};

#endif // !__CAMERA__

