/*************************************************************
**  NAMESPACE: Managers
**  CLASS: Instance
**************************************************************
**  NOTES: Managers is not singleton is just a namespace
**         This class serves for load textures from a file
**************************************************************/

#ifndef _MANAGERS_H_
#define _MANAGERS_H_

#include <iostream>
#include <vector>
#include <map>
#include <cassert>

#include "cfiledef.h"
#include "cgameobject.h"
#include "cterrain.h"
#include "cmesh.h"
#include "ctexture.h"
#include "cmaterial.h"
#include "cshader.h"

using namespace std;

namespace TextureManager
{
	class CTextureManager
	{
		private:
			//Textures map CTexture class and opengl-handler
			std::map < GLuint, CTexture* > m_Textures;    

			//Total num of textures
			GLuint m_uiTextures;

			void clear();

		public:
			CTextureManager();
			CTextureManager(const string file);
			~CTextureManager();
			
			GLuint loadTexture(const string sTexturePath);
			CTexture* getTextureByID(GLuint id);
			bool deleteTexture(GLuint id);
	};

	// Get the instance of texturemanager
	CTextureManager& getTextureManager();
};

#endif
