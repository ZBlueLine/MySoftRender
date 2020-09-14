#pragma once
#ifndef __SHADER__
#define __SHADER__
#include "global.h"
#include "Texture.h"

class Shader {
private:
	int Height;
	int Width;
	Texture* Tex;
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	glm::mat4 PerspectiveMatrix = glm::mat4(1.0f);
public:
	Shader(int w, int h)
		:Width(w), Height(h)
	{
		Tex = new Texture(w, h);
	}
	~Shader()
	{
		delete Tex;
	}
	void LoadTexture(const std::string& Path)
	{
		Tex->LoadTexture(Path);
	}
	void SetModelMatrix(const glm::mat4& Mat)
	{
		ModelMatrix = Mat;
	}

	void SetViewMatrix(const glm::mat4& Mat)
	{
		ViewMatrix = Mat;
	}

	void SetPerspectiveMatrix(const glm::mat4& Mat)
	{
		PerspectiveMatrix = Mat;
	}

	VtoR VertexShader(Vertex& v)
	{
		VtoR resualt;
		resualt.worldp = ModelMatrix * v.position;
		resualt.windowp = ViewMatrix * resualt.worldp;
		resualt.windowp = PerspectiveMatrix * resualt.windowp;

		resualt.Z = 1/resualt.windowp.w;
		resualt.worldp *= resualt.Z;
		resualt.color = v.color * resualt.Z;
		resualt.texcoord = v.texcoord * resualt.Z;
		resualt.normal = v.normal * resualt.Z;
		return resualt;
	}

	glm::vec4 FragmentShader(VtoR v)
	{
		if (Tex->data)
		{
			return Tex->GetColor(glm::vec2(v.texcoord.x, v.texcoord.y));
		}
		return v.color;
	}
};


#endif //!__SHADER__

