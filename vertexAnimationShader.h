
#ifndef __VERTEXANIMATIONSHADER__
#define __VERTEXANIMATIONSHADER__
#include "Vertex.h"
#include "Shader.h"
#include <time.h>
#include <math.h>
class vertexAnimationShader : public Shader
{
	virtual VtoR VertexShader(Vertex& v)
	{
		float time = clock();
		VtoR resualt;
		//v.position = v.position + v.normal; * 0.1f*sin(3.14159f*v.position.x+time*0.01f);
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
		glm::vec4 Col;
		float value = sin(v.worldp.y*30);
		if (value > 0.8)
			return glm::vec4(0);
		if (!Tex->isempty())
			Col = Tex->GetColor(glm::vec2(v.texcoord.x, v.texcoord.y));
		return Col;
	}
};

#endif //!__VERTEXANIMATIONSHADER__