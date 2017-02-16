#include "cgameobject.h"
#include "stdafx.h"

//Buffer index
static GLuint g_bufferId = 0;

//Matrix buffers
static glm::mat4 g_BufferWorldTransformation[MAX_OBJECTS];
static glm::mat4 g_BufferLocalTransformation[MAX_OBJECTS];

#define BUFFER_OFFSET(i) ((unsigned int*) NULL + (i))

CGameObject::CGameObject()
{
	//Clear vectors and initialize buffers
	clear();

	//Initialize the VBO
	Initialize();
}

void CGameObject::clear()
{
	//Id initialization
	m_iId = 0;

	m_type = NO_TYPE;

	//Assign matrix from static matrix´s buffer
	m_WorldTransformation = &g_BufferWorldTransformation[g_bufferId];
	m_LocalTransformation = &g_BufferLocalTransformation[g_bufferId];
	g_bufferId++;

	*m_WorldTransformation = glm::mat4(1.0);
	*m_LocalTransformation = glm::mat4(1.0);

	//Initialize vectors
	m_position = glm::vec3(0.0, 0.0, 0.0);
	m_velocity = glm::vec3(0.0, 0.0, 0.0);
	m_scale = glm::vec3(1.0, 1.0, 1.0);
	m_angle = 0.0;
	m_rotateVector = glm::vec3(0.0, 1.0, 0.0);
	m_bIsVisible = false;
	m_bIsValid = false;

	m_pParent = NULL;
	m_camera = NULL;
	m_world = NULL;

	m_bSmoothShading = false;

	m_handler = 0;
	m_vaoHandle = 0;

	m_hierarchyLevel = LEVEL_0;

	m_iNumVertex = 0;
	m_iNumIndex = 0;
	m_Index.clear();
	m_Vertex.clear();
	m_Normal.clear();
	m_Texel.clear();
	m_vboHandles.clear();

	m_bSmoothShading = false;
}

void CGameObject::Initialize()
{
	//Create a new VAO
	m_handler = 0;

	glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);
}

GLuint CGameObject::getObjectId()
{
	return m_iId;
}

CGameObject::~CGameObject()
{

}

GLuint CGameObject::CreateArrayBuffer(void *buffer, GLuint size, GLenum usage)
{
	// Generates an id for a buffer
	GLuint id = 0;

	glGenBuffers(1, &id);

	m_vboHandles.push_back(id);

	// Store the buffer into GPU
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, usage);

	return id;
}

void CGameObject::CreateAttribArrayBuffer(GLuint attrib_id, GLuint buffer_id, GLuint elements)
{
	// Asociates an attrib (from shader layout) to an array buffer
	glEnableVertexAttribArray(attrib_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glVertexAttribPointer(attrib_id, elements, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}

void CGameObject::BindBufferAttrib(GLuint attrib_id, GLuint buffer_id, GLuint elements)
{
	// Enable the shader attrib
	glEnableVertexAttribArray(attrib_id);

	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

	// Associates the buffer
	glVertexAttribPointer(attrib_id, elements, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}
