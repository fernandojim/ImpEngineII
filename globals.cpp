#include "stdafx.h"

#include <cassert>

#include "cfiledef.h"
#include "globals.h"

using namespace GLOBALS;

/*
	Get the texture manager
*/
CTextureManager& GLOBALS::getTextureManager()
{
	static CTextureManager tm;

	return tm;
}

/*
	Get the material manager
*/
CMaterialManager& GLOBALS::getMaterialManager()
{
	static CMaterialManager tm;

	return tm;
}

/*
	----------------------------
	Methods from CTextureManager
	----------------------------
*/
CTextureManager::CTextureManager()
{
	m_uiTextures = 0;
}

CTextureManager::CTextureManager(const char* file)
{
	// Create and load the file
	m_File = new CFileDef(file);

	//...
}

CTextureManager::~CTextureManager()
{
	std::map < GLuint, CTexture* >::iterator iter = m_Textures.begin();

	for (; iter != m_Textures.end(); ++iter)
	{
		//Gets the object in the i-position
		CTexture* pTexture = iter->second;

		assert(pTexture);

		//Free our resources
		if (pTexture)
		{
			delete pTexture;
			pTexture = NULL;
		}
	}
}

/*
	Load texture from disk and returns its id
*/
GLuint CTextureManager::loadTexture(const std::string sTexturePath)
{
	std::map < GLuint, CTexture* >::iterator iter = m_Textures.begin();

	for (; iter != m_Textures.end(); ++iter)
	{
		//Gets the pair information
		GLuint id = iter->first;
		CTexture* pTexture = iter->second;

		assert(pTexture);

		//Test if the texture is already loaded
		if (!pTexture->getTextureFileName().compare(sTexturePath))
			return pTexture->getID();
	}

	//Add the new texture and its id
	CTexture* pTexture = new CTexture(sTexturePath);
	m_uiTextures = pTexture->getID();
	
	//Test if texture was loaded ok
	if (m_uiTextures > -1 )
		m_Textures.insert(std::pair < GLuint, CTexture* >(m_uiTextures, pTexture));

	return m_uiTextures;
}

/*
	Get texture from index
*/
CTexture* CTextureManager::getTextureID(int id)
{
	std::map < GLuint, CTexture* >::iterator iter;

	//Gets the map of id
	iter = m_Textures.find(id);

	return (CTexture*)iter->second;
}

/*
	Delete texture with ID index
*/
bool CTextureManager::deleteTexture(int id)
{
	std::map < GLuint, CTexture* >::iterator iter;

	//Gets the map of 'id'
	iter = m_Textures.find(id);

	m_Textures.erase(iter);

	return true;
}

/*
    ----------------------------
	Methods for CMaterialManager
	----------------------------
*/
CMaterialManager::CMaterialManager()
{
	m_uiMaterials = 0;
}

CMaterialManager::~CMaterialManager()
{
	std::map < std::string, CMaterial* >::iterator iter = m_Materials.begin();

	for (; iter != m_Materials.end(); ++iter)
	{
		//Gets the object in the i-position
		CMaterial* pMaterial = iter->second;

		assert(pMaterial);

		//Free our resources
		if (pMaterial)
		{
			delete pMaterial;
			pMaterial = NULL;
		}
	}
}

CMaterial* CMaterialManager::getMaterialByName(std::string sName)
{
	std::map < std::string, CMaterial* >::iterator iter;

	//Gets the map of id
	iter = m_Materials.find(sName);

	return (CMaterial*)iter->second;
}
