#ifndef _CLIGHT_H_
#define _CLIGHT_H_

#include "glm\vec3.hpp"

struct light
{
	glm::vec3 lightPosition;
	glm::vec3 lightIntensity;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	float shininess;
};

class CLight
{
public:
	light m_light;

public:
	CLight();
	CLight(light l);
	~CLight();

	void setLight(light l);
	light getLight();
};

#endif
