////////////////////////////////////////////////////////////////////////////////
// CMesh.cpp
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

#include "stdafx.h"

#include "CMesh.h"

#define SZ_MTLLIB sizeof("mtllib")-1
#define SZ_V      sizeof("v ")-1
#define SZ_VT     sizeof("vt")-1
#define SZ_VN     sizeof("vn")-1
#define SZ_F      sizeof("f ")-1
#define SZ_USEMTL sizeof("usemtl")-1
#define SZ_S      sizeof("s")-1

using namespace GLOBALS;

CMesh::CMesh(const std::string name, const std::string object, const std::string vs, const std::string fs) : CObject()
{
	clear();

	m_sName = name;

	load(object);

	m_pShader = new CShader(vs.c_str(), fs.c_str());
}

/*
	Destructor
*/
CMesh::~CMesh()
{
	clear();
}

/*
	Clear data
*/
void CMesh::clear()
{
	m_sName = "";
	m_iNumVertex = 0;
	m_iNumIndex = 0;
	m_Index.clear();
	m_Vertex.clear();
	m_Normal.clear();
	m_Texel.clear();
	m_Faces.clear();
	m_bSmoothShading = false;

	//delete m_pBoundingBox;
	//delete m_pMaterial;
	delete m_pVao;
	delete m_pShader;
}

/*
	Load Material from file
*/
/*bool CMesh::loadMaterial(std::string file)
{
	m_uiTexture = ::getTextureManager().loadTexture(file);

	if (m_uiTexture > 0)
		return true;
	
	return false;
}*/

/*
	Load submesh from file
*/
bool CMesh::load(const std::string file)
{
	m_File = new CFileDef(file);
	
	/* if exist */
	if (m_File && m_File->getIsOpen())
	{
		/* Try read the vertex information in order to create the VAO object */
		loadSubmesh();

		/* Get the size of vector */
		m_iNumVertex = m_iNumIndex = m_Vertex.size();

		/* Create VAO with the vertex arrays loaded */
		//Crea los buffers del objeto VAO
		if (m_iNumVertex > 0)
		{
			m_pVao = new CVao();

			//loadMaterial(m_sTexture);

			m_pVao->CreateArrayBuffer(&m_Vertex[0], m_iNumVertex * sizeof(float)* 3, GL_STATIC_DRAW);
			m_pVao->CreateAttribArrayBuffer(5, 3);

			if (m_Texel.size() > 0)
			{
				m_pVao->CreateArrayBuffer(&m_Texel[0], m_iNumVertex * sizeof(float)* 2, GL_STATIC_DRAW);
				m_pVao->CreateAttribArrayBuffer(6, 2);
			}
			
			if (m_Normal.size() > 0)
			{
				m_pVao->CreateArrayBuffer(&m_Normal[0], m_iNumVertex * sizeof(float)* 3, GL_STATIC_DRAW);
				m_pVao->CreateAttribArrayBuffer(7, 3);
			}
		}
		else
		{
			return false;
		}
						
		/* load was OK */
		return true;
	}
		
	return false;
}

/*
	Read the geometric information from the file
*/
void CMesh::loadSubmesh()
{
	std::string line;
	std::string aux;
	GLuint ind = 0;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> texel;
	std::vector<face> faces;

	/* Clean variables */
	vertex.clear();
	texel.clear();
	normal.clear();
	faces.clear();

	if (m_File->getIsOpen())
	{
		while (!m_File->m_filedef.eof())
		{
			/* get line from file and stores into buffer */
			std::getline(m_File->m_filedef, line);

			/* ignores '#' '\n' '{' or '\0' characters */
			if (line.size() != 0 && line.at(0) != '#' && line.at(0) != '\n' && line.at(0) != '\0')
			{
				//Get the material as
				if (!line.compare(0, SZ_MTLLIB, "mtllib"))
				{
					//The properties of materia will pass to shader
					std::string str(line);
					m_sName = str.substr(7, line.length());
					m_Material = new CMaterial(m_sName);
				}
				else if (!line.compare(0, SZ_V, "v ")) //Vertex information
				{
					vertex.push_back(getVec3Info(line));
				}
				else if (!line.compare(0, SZ_VT, "vt")) //Texel information
				{
					texel.push_back(getVec2Info(line));
				}
				else if (!line.compare(0, SZ_VN, "vn")) //Normal information
				{
					normal.push_back(getVec3Info(line));
				}
				else if (!line.compare(0, SZ_F, "f "))  //Face information
				{
					faces.push_back(getFaceInfo(line));
				}
				else if (!line.compare(0, SZ_USEMTL, "usemtl")) //Texture information
				{
					//std::stringstream(line) >> aux >> m_sTexture;
					std::stringstream(line) >> aux >> m_Material->m_sName;
				}
				else if (!line.compare(0, SZ_S, "s")) // Shader model
				{
					std::stringstream(line) >> aux >> aux;

					if (!aux.compare("off"))
					{
						m_bSmoothShading = false;
					}
					else
					{
						m_bSmoothShading = true;
					}
				}
			}
		}

		/* Creates the buffers */
		//Buffer for vertexs
		for (GLuint i = 0; i < faces.size(); i++)
		{
			for (GLuint j = 0; j < 3; j++)
			{
				m_Vertex.push_back(vertex.at(faces.at(i).vertex[j] - 1));
				m_Index.push_back(ind);
				ind++;
			}
		}

		//Buffer for texels (if exists)
		if (texel.size() > 0)
		{
			for (GLuint i = 0; i < faces.size(); i++)
			{
				for (GLuint j = 0; j < 3; j++)
					m_Texel.push_back(texel.at(faces.at(i).texel[j] - 1));
			}
		}

		//Buffer for normals (if exists)
		if (normal.size() > 0)
		{
			for (GLuint i = 0; i < faces.size(); i++)
			{
				for (GLuint j = 0; j < 3; j++)
					m_Normal.push_back(normal.at(faces.at(i).normal[j] - 1));
			}
		}

		m_File->m_filedef.close();
	}
}

/*
	Get a vec3 info from a cad like "v x y z"
*/
glm::vec3 CMesh::getVec3Info(std::string cad)
{
	glm::vec3 ret;
	std::string aux[4];

	std::stringstream(cad) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
	ret.x = std::stof(aux[1]);
	ret.y = std::stof(aux[2]);
	ret.z = std::stof(aux[3]);

	return ret;
}

/*
	Get a vec2 info from a cad like "v x y" (for texels)
*/
glm::vec2 CMesh::getVec2Info(std::string cad)
{
	glm::vec2 ret;
	std::string aux[3];

	std::stringstream(cad) >> aux[0] >> aux[1] >> aux[2];
	ret.x = std::stof(aux[1]);
	ret.y = std::stof(aux[2]);

	return ret;
}

/*
	Get a face info from a cad like:
	"f 1/1/1 2/2/2 3/3/3" -> vertex/texel/normal
	or "f 1//1 2//2 3//3" -> vertex/normal
	or "f 1/2 3/4 5/6 or  -> vertex texel
	or "f 1 2 3" -> vertex
*/
face CMesh::getFaceInfo(std::string cad)
{
	face f;
	std::string _cad[4];
	std::string token;
	int j;

	/* Get the three vertex info */
	std::stringstream(cad) >> _cad[0] >> _cad[1] >> _cad[2] >> _cad[3];

	for (int i = 0; i <= 2; i++)
	{
		j = 0;
		std::istringstream ss(_cad[i + 1]);
		while (std::getline(ss, token, '/'))
		{
			if (j == 0 && token.size() > 0)
				f.vertex[i] = std::stoi(token);
			else if (j == 1 && token.size() > 0)
				f.texel[i] = std::stoi(token);
			else if (token.size() > 0)
				f.normal[i] = std::stoi(token);

			j++;
		}
	}

	return f;
}

//Animate the submesh or object
void CMesh::Animate(double dt)
{
	
}

//Update the object: Update World Matrix an Bounding box
void CMesh::Update()
{
	//Local object transformation
	*m_LocalTransformation = glm::mat4(1.0); //Initialize local transformations
	*m_LocalTransformation = glm::translate(*m_LocalTransformation, m_position);
	*m_LocalTransformation = glm::rotate(*m_LocalTransformation, m_angle, m_rotateVector);
	*m_LocalTransformation = glm::scale(*m_LocalTransformation, m_scale);

	//If the object has parent, take its matrix
	if (m_pParent)
	{
		*m_WorldTransformation = *m_pParent->m_WorldTransformation;
		*m_LocalTransformation = *m_WorldTransformation * (*m_LocalTransformation);
		*m_WorldTransformation = *m_LocalTransformation;
	}
}

//Render the submesh or object
void CMesh::Render()
{
	GLuint val = 0;
	GLuint offset = 0;
	
	glUseProgram(m_pShader->m_uiProgram);
	{
		//
		//Luces
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.lightPosition");
		glUniform3fv(val, 1, glm::value_ptr(m_world->m_Lights[0].m_light.lightPosition));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.lightIntensity");
		glUniform3fv(val, 1, glm::value_ptr(m_world->m_Lights[0].m_light.lightIntensity));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.ka");
		glUniform3fv(val, 1, glm::value_ptr(m_Material->m_vKa));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.kd");
		glUniform3fv(val, 1, glm::value_ptr(m_Material->m_vKd));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.ks");
		glUniform3fv(val, 1, glm::value_ptr(m_Material->m_vKs));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.shininess");
		glUniform1f(val, m_world->m_Lights[0].m_light.shininess);

		//
		//Matrices
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "m3normal");
		glUniformMatrix3fv(val, 1, GL_FALSE, glm::value_ptr(m_camera->m_M3normal));

		val = glGetUniformLocation(m_pShader->m_uiProgram, "m4modelview");
		glUniformMatrix4fv(val, 1, GL_FALSE, glm::value_ptr(m_camera->m_M4modelView));

		val = glGetUniformLocation(m_pShader->m_uiProgram, "m4mvp");
		glUniformMatrix4fv(val, 1, GL_FALSE, glm::value_ptr(*m_WorldTransformation));

		//
		//Get the texture id for diffuse texture map
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "textureMeshDiffuse");
		glUniform1i(val, m_Material->m_iMap_kd_texture);
		glActiveTexture(GL_TEXTURE0 +  m_Material->m_iMap_kd_texture);
		glBindTexture(GL_TEXTURE_2D,  m_Material->m_iMap_kd_texture);

		glBindVertexArray(m_pVao->m_vaoHandle);
		for (GLuint i = 0; i < (m_iNumIndex / 3); i++)
		{
			offset = i * 3;
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &m_Index[offset]);
		}
		glBindVertexArray(0);
	}
	glUseProgram(0);
}
