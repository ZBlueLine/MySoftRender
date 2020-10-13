#pragma once
#ifndef __GLOBAL__
#define __GLOBAL__

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

class Camera;
class Rasterizer;
class Material;

const glm::vec3 Ambient = glm::vec3(0.5, 0.5, 0.5);
extern Camera* cam;
Material* currentMat;

#endif // !__GLOBAL__

