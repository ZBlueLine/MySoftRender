
#ifndef __SKYBOXSHADER__
#define __SKYBOXSHADER__
#include "Vertex.h"
#include "Shader.h"
#include <time.h>
#include <math.h>
class SkyBoxShader : public Shader
{
protected:
	Texture* TexFront;
	Texture* TexBack;
	Texture* TexTop;
	Texture* TexBottom;
	Texture* TexLeft;
	Texture* TexRight;
	std::vector<Texture*> Cubemap;
public:

	virtual VtoR VertexShader(Vertex& v)
	{
		BackCull = false;
		VtoR resualt;
		resualt.worldp = ModelMatrix * v.position;
		//²»×öÆ½ÒÆ
		ViewMatrix[3][0] = 0;
		ViewMatrix[3][1] = 0;
		ViewMatrix[3][2] = 0;
		resualt.windowp = ViewMatrix * resualt.worldp;
		resualt.windowp = PerspectiveMatrix * resualt.windowp;

		resualt.windowp.z = resualt.windowp.w;
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

#endif //!__VERTEXANIMATIONSHADER__