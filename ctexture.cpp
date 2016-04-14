#include "stdafx.h"

#include "ctexture.h"

#include "SDL_image.h"

CTexture::CTexture(std::string sFilename)
{
	m_Bmp = NULL;

	// Copy the filename into m_filename property
	m_filename = sFilename;
	
	SDL_Surface *image;
	image = IMG_Load ( m_filename.c_str() );

	//Load the BMP associated to the texture
	m_Bmp = new CBmp(m_filename.c_str());
	
	//Test if there was an error reading the bitmap file
	if (m_Bmp && m_Bmp->m_bOk)
	{
		bindTexture();
		m_error = false;
	}
	else
	{
		m_ID = -1;
		m_error = true;
	}
}

CTexture::~CTexture()
{
	if ( m_Bmp )
	{
		delete m_Bmp;
		m_Bmp = NULL;
	}
}

void CTexture::bindTexture()
{
	assert(m_Bmp);

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Bmp->m_lWidth, m_Bmp->m_lHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Bmp->m_pPixelData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

unsigned int CTexture::getID()
{
	if (m_error)
		return 0;
	else
		return m_ID;
}

std::string CTexture::getTextureFileName()
{
	return m_filename;
}
