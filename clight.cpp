#include "stdafx.h"

#include "clight.h"

CLight::CLight()
{
	m_light.ka = glm::vec3(0.0);
	m_light.kd = glm::vec3(0.0);
	m_light.ks = glm::vec3(0.0);
	m_light.shininess = 0.0f;
	m_light.lightIntensity = glm::vec3(0.0);
	m_light.lightPosition = glm::vec3(0.0);
}

CLight::CLight(light l)
{
	m_light.ka = l.ka;
	m_light.kd = l.kd;
	m_light.ks = l.ks;
	m_light.shininess = l.shininess;
	m_light.lightIntensity = l.lightIntensity;
	m_light.lightPosition = l.lightPosition;
}

CLight::~CLight()
{

}

void CLight::setLight(light l)
{
	m_light.ka = l.ka;
	m_light.kd = l.kd;
	m_light.ks = l.ks;
	m_light.shininess = l.shininess;
	m_light.lightIntensity = l.lightIntensity;
	m_light.lightPosition = l.lightPosition;
}

light CLight::getLight()
{
	light l;

	l.ka = m_light.ka;
	l.kd = m_light.kd;
	l.ks = m_light.ks;
	l.shininess = m_light.shininess;
	l.lightIntensity = m_light.lightIntensity;
	l.lightPosition = m_light.lightPosition;

	return l;
}