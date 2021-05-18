#pragma once
#ifndef __SHADER__
#define __SHADER__
#include "global.h"
#include "Texture.h"
#include "CubMap.h"
#include "Camera.h"

class Shader {
protected:
	Texture* Tex;
	glm::vec3 Eyepoint;
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	glm::mat4 PerspectiveMatrix = glm::mat4(1.0f);
	glm::mat3 NormalMatrix = glm::mat3(1.0f);
public:
	bool BackCull;
	Shader()
	{
		BackCull = true;
		Tex = nullptr;
	}
	~Shader() = default;
	
	bool texEmpty()
	{
		if(Tex == nullptr)
			return true;
		return false;
	}
	void SetTexture(Texture* ctex)
	{
		Tex = ctex;
		Tex->GetColor(glm::vec3(0.5, 0.5, 0.5));
	}
	void SetModelMatrix(const glm::mat4& Mat)
	{
		ModelMatrix = Mat;
	}

	void SetViewMatrix(const glm::mat4& Mat)
	{
		ViewMatrix = Mat;
	}

	void SetNormalMatrix(const glm::mat3& Mat) {
		NormalMatrix = Mat;
	}

	void SetPerspectiveMatrix(const glm::mat4& Mat)
	{
		PerspectiveMatrix = Mat;
	}

	void SeteyePoint(const glm::vec3& eye)
	{
		Eyepoint = eye;
	}

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

	//glm::vec4 FragmentShadertt(const VtoR& v)
	//{
	//	if (Tex->data)
	//		glm::vec4 ReturnColor = Tex->GetColor(glm::vec2(v.texcoord.x, v.texcoord.y));
	//	return v.color;
	//}

	struct light
	{
		glm::vec3 position;
		glm::vec3 intensity;
	};

	virtual glm::vec4 FragmentShader(const VtoR& v)
	{
		glm::vec4 return_color = { 0, 0, 0, 0 };
		if (!Tex->isempty())
			return_color = Tex->GetColor(glm::vec2(v.texcoord.x, v.texcoord.y));
		else
			return_color = v.color;
		glm::vec4 texture_color(return_color.x, return_color.y, return_color.z, return_color.w);

		glm::vec3 ka = glm::vec3(0.005, 0.005, 0.005);
		glm::vec3 kd = texture_color;
		glm::vec3 ks = glm::vec3(0.7937, 0.7937, 0.7937);

		auto l1 = light{ {20, -10, 20}, {6, 6, 6} };
		auto l2 = light{ {4, 20, 0}, {6, 6, 6} };

		std::vector<light> lights = { l1, l2 };
		glm::vec3 amb_light_intensity{ 10, 10, 10 };
		glm::vec3 eye_pos = cam->GetPosition();

		float p = 150;

		glm::vec3 color = texture_color;
		glm::vec3 point = v.worldp;
		glm::vec3 normal = v.normal;

		glm::vec3 result_color = { 0, 0, 0 };

		for (auto& light : lights)
		{
			glm::vec3 ambient = kd * ka * amb_light_intensity;
			float r = (light.position - point).length();
			glm::vec3 l = glm::normalize(light.position - point);
			float nl = glm::dot(normal, l);
			glm::vec3 v = glm::normalize(eye_pos - point);
			glm::vec3 h = glm::normalize(v + l);
			
			glm::vec3 diffuse = kd * (light.intensity * float(1.f/pow(r, 2.0))) * std::max(0.f, nl);
			float nh = normal.x * h.x + normal.y * h.y + normal.z * h.z;

			glm::vec3 specular = ks * (light.intensity * float(1.f / pow(r, 2.0)) * pow(std::max(0.f, dot(normal, h)), p));
			result_color += diffuse;
			result_color += specular;
			result_color += ambient;
		}
		return glm::vec4{std::min(1.f,result_color.x), std::min(1.f,result_color.y), std::min(1.f,result_color.z), 0};
	}

};


#endif //!__SHADER__

