
#ifndef __TSTSHADERSHADER__
#define __TSTSHADERSHADER__
#include "Vertex.h"
#include "Shader.h"
class TsTShader : public Shader
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
		glm::vec4 Col;
		if (!Tex->isempty())
			Col = Tex->GetColor(glm::vec2(v.texcoord.x, v.texcoord.y));
		return Col;
	}
};

#endif //!__TSTSHADERSHADER__