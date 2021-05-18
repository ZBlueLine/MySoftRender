#pragma once

#ifndef __MATERIAL__
#define __MATERIAL__

#include "Shader.h"
#include "TextureCUBE.h"
#include "global.h"

class Material {
public:
	glm::vec4 Color;
	glm::vec4 Specular;
	int Gloss; 
	Texture* MainTex;
	Shader* shader;

	Material() :
		Color(glm::vec4(1.0, 1.0, 1.0, 1.0)),
		Specular(glm::vec4(1.0, 1.0, 1.0, 1.0)),
		Gloss(32),
		MainTex(nullptr),
		shader (new Shader)
	{}
	Material(const glm::vec4& color, const glm::vec4& specular, const int& gloss) :
		Color(color),
		Specular(specular),
		Gloss(gloss),
		MainTex(nullptr),
		shader(new Shader)
	{}

	~Material() = default;
	void SetTexture(Texture* t) {
		if (MainTex)
			delete MainTex;
		MainTex = t;
	}
	void SetShader(Shader* s)
	{
		delete shader;
		shader = s;
	}
};


#endif __MATERIAL__