#include "stdafx.h"

#include "clight.h"

CLight::CLight()
{
	m_light.lightIntensity = glm::vec3(0.0);
	m_light.lightPosition = glm::vec3(0.0);
}

CLight::CLight(light l)
{
	m_light.lightIntensity = l.lightIntensity;
	m_light.lightPosition = l.lightPosition;
}

CLight::~CLight()
{

}

void CLight::setLight(light l)
{
	m_light.lightIntensity = l.lightIntensity;
	m_light.lightPosition = l.lightPosition;
}

light CLight::getLight()
{
	light l;

	l.lightIntensity = m_light.lightIntensity;
	l.lightPosition = m_light.lightPosition;

	return l;
}
