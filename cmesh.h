////////////////////////////////////////////////////////////////////////////////
// csubmesh.h
// Author       : Fernando Jiménez Martínez
//
// Copyright    : (C) 2015 by Fernando Jiménez
// Email        : fernandojim@gmail.com
////////////////////////////////////////////////////////////////////////////////

/*********************************************************************************
*                                                                                *
*   This program is free software; you can redistribute it and/or modify         *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or            *
*   (at your option) any later version.                                          *
*                                                                                *
**********************************************************************************/

#ifndef _CMESH_H_
#define _CMESH_H_

#include <vector>
#include <string>

#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm\mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp" //Header for glm::perspective, etc...
#include "cobject.h"
#include "cshader.h"
#include "cvao.h"
#include "cfiledef.h"
#include "globals.h"
#include "cmaterial.h"
#include "cworld.h"
#include "globals.h"

class CWorld;
/*
	Structure for one face. It store only indexes to buffers
*/
struct face
{
	GLuint vertex[3];
	GLuint texel[3];
	GLuint normal[3];
};

class CMesh : public CObject
{
public:
	GLuint m_iNumVertex;
	GLuint m_iNumIndex;
	std::vector<GLuint>    m_Index;
	std::vector<glm::vec3> m_Vertex;
	std::vector<glm::vec3> m_Normal;
	std::vector<glm::vec2> m_Texel;
	std::vector<face>      m_Faces;
	std::string m_sTexture;
	GLuint m_uiTexture_kd;
	GLuint m_uiTexture_ka;
	GLuint m_uiTexture_ks;
	bool m_bSmoothShading; //TRUE=> Smooth Shading on

private:
	CFileDef *m_File;
	
	CMaterial *m_Material;
	//CBoundingBox *m_pBoundingBox;

	CCamera *m_camera;
	CWorld *m_world;

public:
	//Constructor with object and shader files
	CMesh(const std::string name, const std::string object, const std::string vs, const std::string fs);

	//Destructor
	~CMesh();

	//Pointer to game world
	void SetWorld(CWorld *w)    { m_world = w; }

	//Pointer to game camera
	void SetCamera(CCamera *c)  { m_camera = c; }

	//Clear data
	void clear();

	//Animate the submesh
	void Animate(double dt);

	//Update the submesh
	void Update();

	//Render the submesh
	void Render();

private:
	//Methods for load info from object file
	glm::vec3 getVec3Info(std::string cad);
	glm::vec2 getVec2Info(std::string cad);
	face getFaceInfo(std::string cad);
	void loadSubmesh();

	//Load Materials from file
	bool loadMaterial(std::string file);

	//Load submesh from file
	bool load(const std::string file);
};

#endif
