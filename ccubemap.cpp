/*
 * ccubemap.cpp
 *
 *  Created on: 17 de may. de 2017
 *      Author: fjimartinez
 */

#include <SDL_image.h>

#include "ccubemap.h"

using namespace TextureManager;
using namespace RenderManager;

CCubemap::CCubemap(const string cubemap_name)
{
	m_uiCubemapTexture = 0;

	m_M4model = glm::mat4(1.0);
	m_M4view = glm::mat4(1.0);
	m_M4projection = glm::mat4(1.0);

	loadCubeMapData(cubemap_name);
}

bool CCubemap::loadCubeMapData(const string cubemap_name)
{
	CFileDef *filedef = new CFileDef(cubemap_name);

	if (filedef->getIsOpen())
	{
		//Read values from file
		filedef->readObjectKeysValues();

		//Load values from sky map file
		m_Mesh = new CMesh(filedef->getStringObjectValues("MESH")[0]);

		//read texture names
		m_texNames[0] = filedef->getStringObjectValues("FRONT")[0];
		m_texNames[1] = filedef->getStringObjectValues("BACK")[0];
		m_texNames[2] = filedef->getStringObjectValues("UP")[0];
		m_texNames[3] = filedef->getStringObjectValues("DOWN")[0];
		m_texNames[4] = filedef->getStringObjectValues("RIGHT")[0];
		m_texNames[5] = filedef->getStringObjectValues("LEFT")[0];

		//Creates the cube map textures
		loadCubemapTextures();
	}
	else
		return false;

	return true;
}

void CCubemap::loadCubemapTextures()
{
	long width = 0;
	long height = 0;
	BYTE* data = NULL;
	GLenum alignment = GL_RGB;
	GLuint targets[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

	glGenTextures(1, &m_uiCubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiCubemapTexture);

	//Load the textures for all the sides
	for (int i = 0;i < 6;i++)
	{
		SDL_Surface* image = IMG_Load (m_texNames[i].c_str());

		if ( (image->format->Rmask & 0xFF) == 0 )
			alignment = GL_BGR;

		if ( image == NULL || ( image != NULL && ( image->h == 0 || image->w == 0 ) ) ) //Error
		{
			MessageBOX("Loading texture error", SDL_GetError());
		}
		else
		{
			height = image->h;
			width  = image->w;
			data = (BYTE*)image->pixels;

			glTexImage2D(targets[i], 0, GL_RGB, width, height, 0, alignment, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

			delete image;
		}
	}
}

void CCubemap::Render(CCamera *cam)
{
	CShader *skymap_shader = ::getRenderManager().getShaderByName("skymap");

	glDisable(GL_DEPTH_TEST);
	glUseProgram(skymap_shader->m_uiProgram);
	{
		skymap_shader->setUniformShaderTextureCubeMap("cubemapTexture", m_uiCubemapTexture);

		m_M4view = cam->m_M4view;
		m_M4model = glm::translate(cam->position);
		m_M4projection = cam->m_M4projection;
		m_M4mvp = m_M4projection * m_M4view * m_M4model;
		skymap_shader->setUniformShaderMatrix4fv("m4mvp", 1, GL_FALSE, glm::value_ptr(m_M4mvp));

		glBindVertexArray(m_Mesh->m_vaoHandle);

		// Draw the entire buffer
		glDrawArrays(GL_TRIANGLES, 0, m_Mesh->m_iNumIndex);
	}
	glEnable(GL_DEPTH_TEST);
}

CCubemap::~CCubemap()
{
	delete m_Mesh;
}

