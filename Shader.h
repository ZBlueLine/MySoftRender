#pragma once
#ifndef __SHADER__
#define __SHADER__
#include "global.h"

class Shader {
private:
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
public:

	void SetModelMatrix(const glm::mat4& Mat)
	{
		ModelMatrix = Mat;
	}

	void SetViewMatrix(const glm::mat4& Mat)
	{
		ViewMatrix = Mat;
	}

	void SetProjectionMatrix(const glm::mat4& Mat)
	{
		ProjectionMatrix = Mat;
	}

	VtoR VertexShader(Vertex& v)
	{
		VtoR resualt;
		resualt.windowp = ModelMatrix * v.position;
		resualt.windowp = ProjectionMatrix * ViewMatrix * resualt.windowp;

		resualt.color = v.color;
		resualt.texcoord = v.texcoord;
		resualt.normal = v.normal;
		return resualt;
	}

	glm::vec4 FragmentShader(VtoR v)
	{
		return v.color;
	}
};


#endif //!__SHADER__

