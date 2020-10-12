#pragma once

#ifndef __MATERIAL__
#define __MATERIAL__

#include "Shader.h"
#include "Texture.h"
#include "global.h"

class Material {
public:
	glm::vec4 Color;
	glm::vec4 Specular;
	int Gloss;   // 8 - 256
	Texture* MainTex;

	Material() :
		Color(glm::vec4(1.0, 1.0, 1.0, 1.0)),
		Specular(glm::vec4(1.0, 1.0, 1.0, 1.0)),
		Gloss(32),
		MainTex(nullptr)
	{}
	Material(const glm::vec4& color, const glm::vec4& specular, const int& gloss) :
		Color(color),
		Specular(specular),
		Gloss(gloss),
		MainTex(nullptr)
	{}

	~Material() = default;
	void SetTexture(const Texture& t) {
		if (!MainTex)
			MainTex = new Texture;
		*MainTex = t;
	}
};


#endif __MATERIAL__