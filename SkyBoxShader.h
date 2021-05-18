
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
		//也可以在模型中移动好位置
		//处理下接缝的问题
		//v.position -= v.normal * 0.04f;
		resualt.worldp = ModelMatrix * v.position;
		//不做平移
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
			Col = Tex->GetColor(glm::vec2(v.worldp.x, v.worldp.y), v.worldp.z);
		//return glm::vec4(std::fabs(v.worldp.y), 0, 0, 1);
		return Col;
	}
};

#endif //!__VERTEXANIMATIONSHADER__