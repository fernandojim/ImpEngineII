#include "texturemanager.h"
#include "stdafx.h"

#include "cterrain.h"
#include "cmesh.h"

using namespace TextureManager;

/*
	Get the texture manager
*/
CTextureManager& TextureManager::getTextureManager()
{
	static CTextureManager tm;

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

CTextureManager::~CTextureManager()
{
	clear();
}

void CTextureManager::clear()
{
	CTexture* tex = NULL;
	std::map < GLuint, CTexture* >::iterator iter = m_Textures.begin();

	for (; iter != m_Textures.end(); ++iter)
	{
		//Gets the CTexture information in the iter' iteration
		tex = iter->second;

		//Assert texture is correct
		assert(tex);

		//Test if the texture is already loaded
		delete tex;
	}

	m_uiTextures = 0;
	m_Textures.clear();
}

/*
	Load texture from disk and returns its id
*/
GLuint CTextureManager::loadTexture(const std::string sTexturePath)
{
	GLuint last_texture_id = 0;
	CTexture* tex = NULL;
	CTexture* pTexture = NULL;
	std::map < GLuint, CTexture* >::iterator iter = m_Textures.begin();

	//Get the actual id of last texture
	last_texture_id = m_uiTextures;

	for (; iter != m_Textures.end(); ++iter)
	{
		//Gets the CTexture information in the iter' iteration
		tex = iter->second;

	    //Assert texture is correct
	    assert(tex);

		//Test if the texture is already loaded
		if (!tex->getTextureFileName().compare(sTexturePath))
			return tex->getID();
	}

	//Add the new texture and its id
	pTexture = new CTexture(sTexturePath);
	
	m_uiTextures = pTexture->getID();

	//Test if texture was loaded ok
	if ( m_uiTextures > last_texture_id )
		m_Textures.insert(std::pair < GLuint, CTexture* >(m_uiTextures, pTexture));

	return m_uiTextures;
}

/*
	Get texture from index
*/
CTexture* CTextureManager::getTextureByID(GLuint id)
{
	std::map < GLuint, CTexture* >::iterator iter;

	//Gets the map of id
	iter = m_Textures.find(id);

	return (CTexture*)iter->second;
}

/*
	Delete texture with ID index
*/
bool CTextureManager::deleteTexture(GLuint id)
{
	std::map < GLuint, CTexture* >::iterator iter;

	//Gets the map of 'id'
	iter = m_Textures.find(id);

	m_Textures.erase(iter);

	return true;
}








