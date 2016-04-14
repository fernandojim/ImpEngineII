#include "stdafx.h"

#include "cobject.h"

// Índice de buffer
static GLuint g_bufferId = 0;

// BUFFER para las matrices
static glm::mat4 g_BufferWorldTransformation[MAX_BUFFER];
static glm::mat4 g_BufferLocalTransformation[MAX_BUFFER];

CObject::CObject()
{
	//Assign matrix from static matrix´s buffer
	m_WorldTransformation = &g_BufferWorldTransformation[g_bufferId];
	m_LocalTransformation = &g_BufferLocalTransformation[g_bufferId];

	*m_WorldTransformation = glm::mat4(1.0);
	*m_LocalTransformation = glm::mat4(1.0);

	//Initialize vectors
	m_position = glm::vec3(0.0, 0.0, 0.0);
	m_velocity = glm::vec3(0.0, 0.0, 0.0);
	m_scale = glm::vec3(1.0, 1.0, 1.0);
	m_angle = 0.0;
	m_rotateVector = glm::vec3(0.0, 1.0, 0.0);

	g_bufferId++;

	m_pParent = NULL;
}

CObject::~CObject()
{
	delete m_pVao;
	delete m_pShader;
}