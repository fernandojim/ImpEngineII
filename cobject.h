#ifndef _COBJECT_H_
#define _COBJECT_H_

#include <iostream>
#include <fstream>

#include "glm\mat4x4.hpp"
#include "cvao.h"
#include "cshader.h"
#include "ccamera.h"

using std::string;

#define MAX_BUFFER 10

class CObject
{
public:
	/* Name of object */
	string m_sName;

	/* Vertex Array Buffers to GPU */
	CVao	*m_pVao;
	
	/* Shader associated to object */
	CShader *m_pShader;
	
	/* World matrix transformation */
	glm::mat4 *m_WorldTransformation;

	/* Local matrix transformation */
	glm::mat4 *m_LocalTransformation;

	/* Pointer to parent object */
	CObject *m_pParent;

	/* Is visible? */
	bool m_bIsVisible;

	glm::vec3 m_position;
	glm::vec3 m_rotateVector;
	glm::vec3 m_scale;
	float m_angle;
	glm::vec3 m_velocity;

public:
	CObject();
	~CObject();

	virtual void Animate(double dt) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};

#endif