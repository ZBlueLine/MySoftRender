
#ifndef __VERTEXANIMATIONSHADER__
#define __VERTEXANIMATIONSHADER__
#include "Vertex.h"
#include "Shader.h"
class vertexAnimationShader : public Shader
{
	virtual VtoR VertexShader(Vertex& v)
	{
		VtoR resualt;
		//v.position += v.normal * glm::vec4(0.5, 0.5, 0.5, 0);
		resualt.worldp = ModelMatrix * v.position;
		resualt.windowp = ViewMatrix * resualt.worldp;
		resualt.windowp = PerspectiveMatrix * resualt.windowp;
		resualt.normal = glm::normalize(NormalMatrix * v.normal);

		resualt.texcoord = v.texcoord;
		resualt.color = v.color;

		return resualt;

	}

	virtual glm::vec4 FragmentShader(const VtoR& v)
	{
		return glm::vec4(0.3, 0.4, 0.5, 1);
	}
};

#endif //!__VERTEXANIMATIONSHADER__