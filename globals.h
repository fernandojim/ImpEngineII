/*************************************************************
**  NAMESPACE: TextureLibrary
**  CLASS: Instance
**************************************************************
**  NOTES: Texture library is not singleton is just a namespace
**         This class serves for load textures from a file
**************************************************************/

#ifndef _NTEXTUREMANAGER_H_
#define _NTEXTUREMANAGER_H_

#include <iostream>
#include <map>

#include "ctexture.h"
#include "cmaterial.h"
#include "cfiledef.h"

using namespace std;

namespace GLOBALS
{
	class CTextureManager
	{
		private:
			//Textures map CTexture class and opengl-handler
			std::map < GLuint, CTexture* > m_Textures;    

			//Total num of textures
			GLuint m_uiTextures;

			//File for load textures
			CFileDef *m_File = NULL;

		public:
			CTextureManager();
			CTextureManager(const char* file);
			~CTextureManager();
			
			GLuint loadTexture(const std::string sTexturePath);
			
			CTexture* getTextureID(int id);
			
			bool deleteTexture(int id);
	};

	class CMaterialManager
	{
		private:
			//Material mapping by name
			std::map < std::string, CMaterial* > m_Materials;

			//Total num of materials
			GLuint m_uiMaterials;

		public:
			CMaterialManager();
			~CMaterialManager();

			CMaterial* getMaterialByName(std::string sName);

			bool deleteMaterial(std::string sName);

	};

	// Get the instance of texturemanager
	CTextureManager& getTextureManager();

	// Get the instance of materialmanager
	CMaterialManager& getMaterialManager();
};

#endif