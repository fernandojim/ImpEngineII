#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_

#include <iostream>
#include <fstream>
#include <vector>

#include "glm\mat4x4.hpp"
#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm\gtc\quaternion.hpp"
#include "cvao.h"
#include "nvertex.h"

class CCamera;
class CWorld;

using namespace NVERTEX;
using std::string;

const GLuint MAX_OBJECTS = 500;

enum OBJECT_TYPE {NO_TYPE = 0, TERRAIN, BLENDER, MD2};

enum OBJECT_HIERARCHY_LEVEL {LEVEL_0 = 0, LEVEL_1, LEVEL_2, LEVEL_3};

struct face
{
	GLuint nverts;
	GLuint vertex[4];
	GLuint texel[4];
	GLuint normal[4];
};

/*
 * Main class for the game objects
 */
class CGameObject
{
public:
	/* unique id of object */
	GLuint m_iId;

	/* Game camera */
	CCamera *m_camera;

	/* Game world */
	CWorld   *m_world;

	//Geometric info for VBOs
	GLuint m_iNumVertex;
	GLuint m_iNumIndex;
	std::vector<GLuint>    m_Index;
	std::vector<glm::vec3> m_Vertex;
	std::vector<glm::vec3> m_Normal;
	std::vector<glm::vec2> m_Texel;
	std::vector<face>      m_Faces;
	bool m_bSmoothShading; //TRUE=> Smooth Shading on

	/* World matrix transformation */
	glm::mat4 *m_WorldTransformation;

	/* Local matrix transformation */
	glm::mat4 *m_LocalTransformation;

	/* Pointer to parent object */
	CGameObject *m_pParent;

	/* Is visible? */
	bool m_bIsVisible;

	/* Is valid? */
	bool m_bIsValid;

	/* Level in the hierarchy */
	OBJECT_HIERARCHY_LEVEL m_hierarchyLevel;

	/* Object type */
	OBJECT_TYPE m_type;

	/* VAO */
	GLuint m_vaoHandle;
	std::vector<GLuint> m_vboHandles;
	GLuint m_handler;

	// Method for create Arrays of Buffer Objects
	GLuint CreateArrayBuffer(void *buffer, GLuint size, GLenum usage);

	// Method for create an attrib to an array buffer to Shader
	void CreateAttribArrayBuffer(GLuint attrib_id, GLuint buffer_id, GLuint elements);

	// Method for bind a buffer to a shader attrib
	void BindBufferAttrib(GLuint attrib_id, GLuint buffer_id, GLuint elements);

public:
	/* Physics */
	glm::vec3 m_acceleration;
	glm::vec3 m_velocity;
	glm::vec3 m_position;
	glm::vec3 m_rotateVector;
	glm::vec3 m_scale;
	glm::vec3 m_facing;
	GLfloat   m_speed;
	float     m_angle;

	/* Rotation quaterions */
	glm::vec3 m_angles;
	glm::quat m_qrot;
	glm::quat m_qrotTo;

protected:
	GLuint getObjectId();

public:
	CGameObject();
	virtual ~CGameObject() = 0;

	void clear();
	void Initialize();

	//Pointer to game camera
	void SetCamera(CCamera *c)  { m_camera = c; }

	//Pointer to game world
	void SetWorld(CWorld *w)    { m_world = w; }
};

#endif
