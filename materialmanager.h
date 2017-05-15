/*
 * materialmanager.h
 *
 *  Created on: 26 de oct. de 2016
 *      Author: fjimartinez
 */

#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

#include "stdafx.h"

#include <iostream>
#include <map>

#include "cfiledef.h"
#include "cmaterial.h"
#include "texturemanager.h"

using namespace std;

namespace MaterialManager
{
	enum TEXTURE_MAP_TYPE {MAP_KA = 0, MAP_KD, MAP_KS};

	class CMaterialManager
	{
		private:
			//Material mapping by name
			std::map < string, CMaterial* > m_Materials;

			//Total num of materials
			GLuint m_uiMaterials;

		public:
			CMaterialManager();
			~CMaterialManager();

			void loadMaterialsFromMTL(const string sMaterialPath);

			CMaterial* getMaterialByName(const string sName);
			GLuint getTextureId(TEXTURE_MAP_TYPE texType, const string sName);

			bool deleteMaterial(const string sName);

	};

	// Get the instance of materialmanager
	CMaterialManager& getMaterialManager();

}

#endif /* MATERIALMANAGER_H_ */
