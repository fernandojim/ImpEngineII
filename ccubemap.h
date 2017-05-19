/*
 * ccubemap.h
 *
 *  Created on: 17 de may. de 2017
 *      Author: fjimartinez
 */

#ifndef CCUBEMAP_H_
#define CCUBEMAP_H_

#include "stdafx.h"

#include "texturemanager.h"
#include "rendermanager.h"
#include "cmesh.h"

class CMesh;

class CCubemap
{
public:
	CCubemap(const string cubemap_name);
	~CCubemap();

	void Render(CCamera *cam);

	GLuint m_uiCubemapTexture;

private:
	bool loadCubeMapData(const string cubemap_name);
	void loadCubemapTextures();

	CMesh *m_Mesh;

	glm::mat4 m_M4model;
	glm::mat4 m_M4view;
	glm::mat4 m_M4projection;
	glm::mat4 m_M4mvp;

	string m_texNames[6];
};

#endif /* CCUBEMAP_H_ */
