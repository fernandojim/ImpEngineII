#include "stdafx.h"

#include "ctexture.h"
#include "utils.h"

#include <SDL_image.h>

using namespace Message;

CTexture::CTexture(std::string sFilename)
{
	CBmp *bmp = NULL;

	// Copy the filename into m_filename property
	m_filename = sFilename;
	
	// No error at the beginning
	m_error = false;

	// If the texture is a JPG file
	if (m_filename.find(".jpg") != string::npos || m_filename.find(".JPG") != string::npos)
	{
		//Load the JPG associated to the texture
		SDL_Surface* image = IMG_Load ( m_filename.c_str() );

		if ( image == NULL || ( image != NULL && ( image->h == 0 || image->w == 0 ) ) ) //Error
		{
			m_ID = -1;
			m_error = true;
		}
		else
		{
			m_lHeight = image->h;
			m_lWidth  = image->w;
			m_Pixeldata = (BYTE*)image->pixels;
		}
	}
	else if (m_filename.find(".bmp") != string::npos)
	{
		//Load the BMP associated to the texture
		bmp = new CBmp(m_filename.c_str());

		if ( bmp == NULL || ( bmp != NULL && ( bmp->m_lHeight == 0 || bmp->m_lWidth == 0) ) )
		{
			m_ID = -1;
			m_error = true;
		}
		else
		{
			m_lHeight = bmp->m_lHeight;
			m_lWidth  = bmp->m_lWidth;
			m_Pixeldata = bmp->m_pPixelData;
		}
	}

	// Test if error
	if ( m_error )
	{
		MessageBOX(m_filename.c_str(), "Texture loader error");
	}
	else
	{
		//Bind  and creates an id for the texture
		bindTexture();
		m_error = false;
	}
}

CTexture::~CTexture()
{
	if ( m_Pixeldata )
	{
		delete [] m_Pixeldata;
		m_Pixeldata = NULL;
	}
}

void CTexture::bindTexture()
{
	assert(m_Pixeldata);

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_lWidth, m_lHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Pixeldata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

GLuint CTexture::getID()
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
