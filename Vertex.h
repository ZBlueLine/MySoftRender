#pragma once
#ifndef __VERTEX__
#define __VERTEX__

#include <iostream>
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
		const glm::vec4& c = glm::vec4(0, 0, 0, 0),
		const glm::vec4& n = glm::vec4(0, 0, 0, 0),
		const glm::vec2& t = glm::vec2(0, 0))
		:position(p), color(c), normal(n), texcoord(t)
	{}
	Vertex(const glm::vec3& p,
		const glm::vec4& c = glm::vec4(0, 0, 0, 0),
		const glm::vec4& n = glm::vec4(0, 0, 0, 0),
		const glm::vec2& t = glm::vec2(0, 0))
		:position(p, 1.0f), color(c), normal(n), texcoord(t)
	{}

};


class VtoR {
public:
	glm::vec4 worldp;
	glm::vec4 windowp;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
	float Z;
	VtoR() = default;

	friend std::ostream& operator<< (std::ostream &os, const VtoR& v);

	static VtoR Lerp(const VtoR& v1, const VtoR& v2, const float& weigh)
	{
		VtoR resualt;
		resualt.worldp = lerp(v1.worldp, v2.worldp, weigh);
		resualt.windowp = lerp(v1.windowp, v2.windowp, weigh);
		resualt.color = lerp(v1.color, v2.color, weigh);
		resualt.texcoord = lerp(v1.texcoord, v2.texcoord, weigh);
		resualt.normal = lerp(v1.normal, v2.normal, weigh);
		resualt.Z = lerp(v1.Z, v2.Z, weigh);

		return resualt;
	}
};

std::ostream& operator<<(std::ostream &os, const VtoR& v)
{
	os << "Windowp" << std::endl;
	os << v.windowp.x << ' ' << v.windowp.y << ' ' << v.windowp.z <<' '<<v.windowp.w<< std::endl;
	os << "color" << std::endl;;
	os << v.color.r << ' ' << v.color.g << ' ' << v.color.b << std::endl;;

	return os;
}

#endif // !__VERTEX__

