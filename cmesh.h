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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm\mat4x4.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp" //Header for glm::perspective, etc...
#include "cshader.h"
#include "cmaterial.h"
#include "cworld.h"
#include "cfiledef.h"
#include "cgameobject.h"
#include "texturemanager.h"
#include "materialmanager.h"

class CWorld;
/*
	Structure for one face. It store only indexes to buffers
*/

class CMesh : public CGameObject
{
public:
	CMaterial *m_Material;

private:
	CFileDef* m_File;

public:
	//Constructor with object and shader files
	CMesh();
	CMesh(const string name);

	//Destructor
	~CMesh();

	//Animate the submesh
	void Animate(double dt);

	//Update the submesh
	void Update();

	//Render the submesh
	void Render();

private:
	//Methods for load info from object file
	glm::vec3 getVec3Info(string cad);
	glm::vec2 getVec2Info(string cad);
	face getFaceInfo(string cad);
	void loadMesh();

	//Load submesh from file
	bool load(const string file);
};

#endif
