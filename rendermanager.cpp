/*
 * rendermanager.cpp
 *
 *  Created on: 26 de oct. de 2016
 *      Author: fjimartinez
 */

#include "rendermanager.h"
#include "gameobjectmanager.h"

using namespace RenderManager;
using namespace GameObjectManager;

/*
	Get the shader manager
*/
CRenderManager& RenderManager::getRenderManager()
{
	static CRenderManager tm;

	return tm;
}

/*
    ----------------------------
	Methods for CRenderManager
	----------------------------
*/
CRenderManager::CRenderManager()
{
	m_uiShaders = 0;

	m_Shaders.clear();
}

CRenderManager::~CRenderManager()
{
	std::map < string, CShader* >::iterator iter = m_Shaders.begin();

	for (; iter != m_Shaders.end(); ++iter)
	{
		delete iter->second;
	}

	m_Shaders.clear();
}

void CRenderManager::loadShadersFromFile(const string sShaders)
{
	CShader* pShader = NULL;
	string shader_name;
	string vert;
	string frag;

	// Create the file to read info
	CFileDef *filedef = new CFileDef(sShaders);

	// If is open
	if (filedef && filedef->getIsOpen())
	{
		// Read the info
		filedef->readObjectKeysValues();

		/* Gets the values loaded from file */
		for (int i = 0;i<filedef->m_numkeys;i++)
		{
			shader_name = filedef->getObjectKey(i);

			// Make .vertex and .fragment names and creates the shader
			vert = "shaders\\" + shader_name + ".vertex";
			frag = "shaders\\" + shader_name + ".fragment";

			// Creates the shader
			pShader = new CShader(vert.c_str(), frag.c_str());

			//Add shader to list
			m_Shaders.insert(std::pair < string, CShader* >(shader_name, pShader));
			m_uiShaders++;
		}

		delete filedef;
	}
}

CShader* CRenderManager::getShaderByName(const string sName)
{
	CShader* pShader = NULL;

	//Creates the iterator
	std::map<std::string, CShader*>::iterator p = m_Shaders.find(sName);

	if (p != m_Shaders.end())
	   pShader = p->second;

	return pShader;
}

void CRenderManager::Render()
{
	::getGameObjectManager().m_fbo->BindFBO();
	getRenderManager().Render(getGameObjectManager().m_GameObjectTerrain);
	//getRenderManager().Render(getGameObjectManager().m_GameObjectsMesh);
	getRenderManager().Render(getGameObjectManager().m_GameObjectsMD2);
	::getGameObjectManager().m_fbo->UnbindFBO();
	glViewport(0,0,800,600);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	getRenderManager().Render(getGameObjectManager().m_GameObjectTerrain);
	getRenderManager().Render(getGameObjectManager().m_GameObjectsMesh);
	getRenderManager().Render(getGameObjectManager().m_GameObjectsMD2);
}

void CRenderManager::Render(std::vector < CTerrain > & terrains)
{
	GLint offset;
	GLint size;

	//
	//Load the shader for terrain
	//
	CShader *terrain_shader = getShaderByName("terrain");

	//
	//Draw terrain object
	//
	glUseProgram(terrain_shader->m_uiProgram);
	{
		for ( auto &i : terrains )
		{
		     CTerrain *terr = &i;

		    //
		    //frameBuffer texture
		    //
		    terrain_shader->setUniformShaderTexture2D("texturafrmb", ::getGameObjectManager().m_fbo->m_uiTextureHandler);

		    //
			//Tiling texture factors, etc.
			//
			terrain_shader->setUniformShader1f("tilingFactorSand", terr->m_tilingFactorSand);
			terrain_shader->setUniformShader1f("tilingFactorGrass", terr->m_tilingFactorGrass);
			terrain_shader->setUniformShader1f("tilingFactorDirt", terr->m_tilingFactorDirt);
			terrain_shader->setUniformShader1f("tilingFactorSnow", terr->m_tilingFactorSnow);
			terrain_shader->setUniformShader1f("altmax", terr->m_fAlturaMax);

			//
			//Camera position
			//
			terrain_shader->setUniformShader3fv("camera_pos", 1, glm::value_ptr(terr->m_camera->position));

			//
			//Lights
			//
			terrain_shader->setUniformShader3fv("luz1.lightPosition", 1, glm::value_ptr(terr->m_world->m_Lights[0].m_light.lightPosition));
			terrain_shader->setUniformShader3fv("luz1.lightIntensity", 1, glm::value_ptr(terr->m_world->m_Lights[0].m_light.lightIntensity));

			//
			//Materials
			//
			// sand
			terrain_shader->setUniformShader1f("material1.shininess", terr->m_MatSand.m_fNs);
			terrain_shader->setUniformShader3fv("material1.ka", 1, glm::value_ptr(terr->m_MatSand.m_vKa));
			terrain_shader->setUniformShader3fv("material1.kd", 1, glm::value_ptr(terr->m_MatSand.m_vKd));
			terrain_shader->setUniformShader3fv("material1.ks", 1, glm::value_ptr(terr->m_MatSand.m_vKs));
			terrain_shader->setUniformShaderTexture2D("material1.tex_kd", terr->m_MatSand.m_iMap_kd_texture);

			// grass
			terrain_shader->setUniformShader1f("material2.shininess", terr->m_MatGrass.m_fNs);
			terrain_shader->setUniformShader3fv("material2.ka", 1, glm::value_ptr(terr->m_MatGrass.m_vKa));
			terrain_shader->setUniformShader3fv("material2.kd", 1, glm::value_ptr(terr->m_MatGrass.m_vKd));
			terrain_shader->setUniformShader3fv("material2.ks", 1, glm::value_ptr(terr->m_MatGrass.m_vKs));
			terrain_shader->setUniformShaderTexture2D("material2.tex_kd", terr->m_MatGrass.m_iMap_kd_texture);

			// dirt
			terrain_shader->setUniformShader1f("material3.shininess", terr->m_MatDirt.m_fNs);
			terrain_shader->setUniformShader3fv("material3.ka", 1, glm::value_ptr(terr->m_MatDirt.m_vKa));
			terrain_shader->setUniformShader3fv("material3.kd", 1, glm::value_ptr(terr->m_MatDirt.m_vKd));
			terrain_shader->setUniformShader3fv("material3.ks", 1, glm::value_ptr(terr->m_MatDirt.m_vKs));
			terrain_shader->setUniformShaderTexture2D("material3.tex_kd", terr->m_MatDirt.m_iMap_kd_texture);

			// snow
			terrain_shader->setUniformShader1f("material4.shininess", terr->m_MatSnow.m_fNs);
			terrain_shader->setUniformShader3fv("material4.ka", 1, glm::value_ptr(terr->m_MatSnow.m_vKa));
			terrain_shader->setUniformShader3fv("material4.kd", 1, glm::value_ptr(terr->m_MatSnow.m_vKd));
			terrain_shader->setUniformShader3fv("material4.ks", 1, glm::value_ptr(terr->m_MatSnow.m_vKs));
			terrain_shader->setUniformShaderTexture2D("material4.tex_kd", terr->m_MatSnow.m_iMap_kd_texture);

			//
			//Transformation Matrices
			//
			terrain_shader->setUniformShaderMatrix3fv("m3normal", 1, GL_FALSE, glm::value_ptr(terr->m_camera->m_M3normal));
			terrain_shader->setUniformShaderMatrix4fv("m4modelview", 1, GL_FALSE, glm::value_ptr(terr->m_camera->m_M4modelView));
			terrain_shader->setUniformShaderMatrix4fv("m4mvp", 1, GL_FALSE, glm::value_ptr(*terr->m_WorldTransformation));
			terrain_shader->setUniformShaderMatrix4fv("m4vpi", 1, GL_FALSE, glm::value_ptr(terr->m_camera->m_M4ViewProjectionInverse));

			//
			//Render the object terrain
			//
			glBindVertexArray(terr->m_vaoHandle);
			size = terr->m_iDivisiones * 2;
			for (GLuint i = 0 ; i < terr->m_iDivisiones - 1 ; i++)
			{
				offset = size * i;
				glDrawElements(GL_TRIANGLE_STRIP, size, GL_UNSIGNED_INT, &terr->m_Index[offset]);
			}
		}
	}
}

void CRenderManager::Render(std::vector < CMesh > & meshes)
{
	//
	//Load the shader for meshes
	//
	CShader *mesh_shader = getShaderByName("mesh1");

	//
	//Draw meshes objects
	//
	glUseProgram(mesh_shader->m_uiProgram);
	{
		for ( auto &i : meshes )
		{
		     CMesh *mesh = &i;

		     //
		     //Textures
		     //
		     mesh_shader->setUniformShaderTexture2D("textureMeshDiffuse", mesh->m_Material->m_iMap_kd_texture);
		     //mesh_shader->setUniformShaderTexture2D("textureMeshDiffuse", ::getGameObjectManager().m_fbo->m_uiTextureHandler);

		     //
			 //Lights
			 //
			 mesh_shader->setUniformShader3fv("luz1.lightPosition", 1, glm::value_ptr(mesh->m_world->m_Lights[0].m_light.lightPosition));
			 mesh_shader->setUniformShader3fv("luz1.lightIntensity", 1, glm::value_ptr(mesh->m_world->m_Lights[0].m_light.lightIntensity));

			 //
			 //Materials
			 //
			 mesh_shader->setUniformShader3fv("material1.ka", 1, glm::value_ptr(mesh->m_Material->m_vKa));
			 mesh_shader->setUniformShader3fv("material1.kd", 1, glm::value_ptr(mesh->m_Material->m_vKd));
			 mesh_shader->setUniformShader3fv("material1.ks", 1, glm::value_ptr(mesh->m_Material->m_vKs));
			 mesh_shader->setUniformShader1f("material1.shininess", mesh->m_Material->m_fNs);

			 //
			 //Transformation Matrices
			 //
			 mesh_shader->setUniformShaderMatrix3fv("m3normal", 1, GL_FALSE, glm::value_ptr(mesh->m_camera->m_M3normal));
			 mesh_shader->setUniformShaderMatrix4fv("m4modelview", 1, GL_FALSE, glm::value_ptr(mesh->m_camera->m_M4modelView));
			 mesh_shader->setUniformShaderMatrix4fv("m4mvp", 1, GL_FALSE, glm::value_ptr(*mesh->m_WorldTransformation));

			//
			//Render the object terrain
			//
			// Bind the Vertex Array Object 'VBO'
			glBindVertexArray(mesh->m_vaoHandle);

			// Draw the entire buffer
			glDrawArrays(GL_TRIANGLES, 0, mesh->m_iNumIndex);
		}
	}
}

void CRenderManager::Render(std::vector < CMd2model > & md2)
{
	int nframe = 0;

	//
	//load the shader for md2 models
	//
	CShader *md2model_shader = getShaderByName("md2model");

	//
	//Draw meshes objects
	//
	glUseProgram(md2model_shader->m_uiProgram);
	{
		for ( auto &i : md2 )
		{
			CMd2model *md2model = &i;

			// Get the current frame
			// Check if n is in a valid range */
			nframe = md2model->getActiveFrame();
			if ((nframe < 0) || (nframe > md2model->m_header.num_frames))
				return;

			//
			//Texture
			//
			md2model_shader->setUniformShaderTexture2D("textureMeshDiffuse", md2model->m_idTexture);

			//
			//Lights
			//
			glm::vec3 ka = glm::vec3(1.0, 1.0, 1.0);
			glm::vec3 kd = glm::vec3(1.0, 1.0, 1.0);
			glm::vec3 ks = glm::vec3(0.5, 0.5, 0.5);
			float shininess = 1.0;
			md2model_shader->setUniformShader3fv("luz1.lightPosition", 1, glm::value_ptr(md2model->m_world->m_Lights[0].m_light.lightPosition));
			md2model_shader->setUniformShader3fv("luz1.lightIntensity", 1, glm::value_ptr(md2model->m_world->m_Lights[0].m_light.lightIntensity));
			md2model_shader->setUniformShader3fv("material1.ka", 1, glm::value_ptr(ka));
			md2model_shader->setUniformShader3fv("material1.kd", 1, glm::value_ptr(kd));
			md2model_shader->setUniformShader3fv("material1.ks", 1, glm::value_ptr(ks));
			md2model_shader->setUniformShader1f("material1.shininess", shininess);

			//
			//Transformation Matrices
			//
			md2model_shader->setUniformShaderMatrix3fv("m3normal", 1, GL_FALSE, glm::value_ptr(md2model->m_camera->m_M3normal));
			md2model_shader->setUniformShaderMatrix4fv("m4modelview", 1, GL_FALSE, glm::value_ptr(md2model->m_camera->m_M4modelView));
			md2model_shader->setUniformShaderMatrix4fv("m4mvp", 1, GL_FALSE, glm::value_ptr(*md2model->m_WorldTransformation));

			//
			//Interpolation
			//
			md2model_shader->setUniformShader1f("fInterpolation", md2model->m_AnimateState.interpolation);

			//
			//Render the object terrain
			//
			// Bind the Vertex Array Object 'VBO'
			glBindVertexArray(md2model->m_vaoHandle);

			// Bind the buffers corresponding to frame and next frame
			md2model->BindBufferAttrib(8, md2model->frame_vnBuffer[md2model->m_AnimateState.curr_frame].vBuffer, 3);
			md2model->BindBufferAttrib(9, md2model->frame_vnBuffer[md2model->m_AnimateState.next_frame].vBuffer, 3);
			md2model->BindBufferAttrib(10, md2model->frame_vnBuffer[md2model->m_AnimateState.curr_frame].nBuffer, 3);
			md2model->BindBufferAttrib(11, md2model->frame_vnBuffer[md2model->m_AnimateState.next_frame].nBuffer, 3);

			// Bind the texel buffer
			md2model->BindBufferAttrib(12, md2model->m_iTexelBuffer, 2);

			// Draw the entire buffers
			glDrawArrays(GL_TRIANGLES, 0, md2model->m_vertsPerFrame);
		}
	}
}

