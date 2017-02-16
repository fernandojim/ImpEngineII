/*
 * rendermanager.h
 *
 *  Created on: 26 de oct. de 2016
 *      Author: fjimartinez
 */

#ifndef RENDERMANAGER_H_
#define RENDERMANAGER_H_

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <map>

#include "cshader.h"
#include "cterrain.h"
#include "cmesh.h"
#include "cmd2model.h"

using namespace std;

class CTerrain;
class CMesh;
class CMd2model;

namespace RenderManager
{
	class CRenderManager
	{
		private:
			//Material mapping by name
			std::map < std::string, CShader* > m_Shaders;

			//Total num of shaders
			GLuint m_uiShaders;

		public:
			CRenderManager();
			~CRenderManager();

			void loadShadersFromFile(const string sShaders);

			CShader* getShaderByName(const string sName);

			void Render();

		private:
			void Render(std::vector < CMd2model > & md2);
			void Render(std::vector < CMesh > & meshes);
			void Render(std::vector < CTerrain > & terrains);
	};

	// Get the instance of renderermanager
	CRenderManager& getRenderManager();
};

#endif /* RENDERMANAGER_H_ */
