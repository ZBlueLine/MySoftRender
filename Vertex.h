#pragma once
#ifndef __VERTEX__
#define __VERTEX__
#include "global.h"
#include "Math.h"

class Vertex {
public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec4 normal;

	Vertex() = default;
	Vertex(const glm::vec4 &p, 
		const glm::vec4 &c = glm::vec4(0, 0, 0, 0), 
		const glm::vec2 &t = glm::vec2(0, 0),
		const glm::vec4 &n = glm::vec4(0, 0, 0, 0))
		:position(p), color(c), texcoord(t), normal(n)
	{}
	Vertex(const glm::vec3& p,
		const glm::vec4& c = glm::vec4(0, 0, 0, 0),
		const glm::vec2& t = glm::vec2(0, 0),
		const glm::vec4& n = glm::vec4(0, 0, 0, 0))
		:position(p, 1.0f), color(c), texcoord(t), normal(n)
	{}

};


class VtoR {
public:
	glm::vec4 worldp;
	glm::vec4 windowp;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec4 normal;
	VtoR() = default;
	static VtoR Lerp(const VtoR& v1, const VtoR& v2, const float& weigh)
	{
		VtoR resualt;
		resualt.worldp = lerp(v1.worldp, v2.worldp, weigh);
		resualt.windowp = lerp(v1.windowp, v2.windowp, weigh);
		resualt.color = lerp(v1.color, v2.color, weigh);
		resualt.texcoord = lerp(v1.texcoord, v2.texcoord, weigh);
		resualt.normal = lerp(v1.normal, v2.normal, weigh);
		return resualt;
	}
};

#endif // !__VERTEX__

