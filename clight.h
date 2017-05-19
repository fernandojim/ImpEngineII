#ifndef _CLIGHT_H_
#define _CLIGHT_H_

#include "glm\vec3.hpp"

struct light
{
	glm::vec3 lightPosition;
	glm::vec3 lightIntensity;
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
