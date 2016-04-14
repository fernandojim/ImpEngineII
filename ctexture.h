#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include <iostream>
#include <fstream>
#include <cassert>

#include "gl_core_3_3.h"

#include "cbmp.h"

using namespace std;

class CBmp;

class CTexture
{
private:
	unsigned int m_ID;			//Texture id in OpengGL context
	std::string  m_filename;	//Texture file name
	bool         m_error;		//If there is an error
	CBmp	    *m_Bmp;

public:
	CTexture(std::string sFilename);	//Constructor
	~CTexture();						//Destructor

	void bindTexture();					//Load the texture into OpenGL
	unsigned int getID();
	std::string getTextureFileName();
};

#endif