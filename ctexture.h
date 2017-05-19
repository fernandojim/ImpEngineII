#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include <iostream>
#include <fstream>
#include <cassert>

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

#include "cbmp.h"

using namespace std;

class CBmp;

class CTexture
{
private:
	unsigned int m_ID;			//Texture id in OpengGL context
	std::string  m_filename;	//Texture file name
	bool         m_error;		//If there is an error

	//Generic info for texture image
	long         m_lHeight;
	long         m_lWidth;
	BYTE        *m_Pixeldata;

	// RGB or BGR
	GLenum       m_rgbAlignment; //GL_RGB or GL_BGR

public:
	CTexture(std::string sFilename);	//Constructor
	~CTexture();						//Destructor

	void bindTexture();					//Load the texture into OpenGL
	unsigned int getID();
	std::string getTextureFileName();
};

#endif
