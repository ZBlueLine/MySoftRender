#pragma once
#ifndef __TRIANGLE__
#define __TRIANGLE__
#include "global.h"
#include "Vertex.h"

class Triangle {

public:
	Vertex v1;
	Vertex v2;
	Vertex v3;
	Triangle(const Vertex& o1, const Vertex& o2, const Vertex& o3)
		: v1(o1), v2(o2), v3(o3)
	{}
	~Triangle() = default;
};

#endif // !__TRIANGLE__

