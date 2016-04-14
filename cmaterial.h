////////////////////////////////////////////////////////////////////////////////
// cmaterial.h
// Author       : Fernando Jiménez Martínez
//
// Copyright    : (C) 2015 by Fernando Jiménez
// Email        : fernandojim@gmail.com
////////////////////////////////////////////////////////////////////////////////

/*********************************************************************************
*                                                                                *
*   This program is free software; you can redistribute it and/or modify         *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or            *
*   (at your option) any later version.                                          *
*                                                                                *
**********************************************************************************/

#ifndef _CMATERIAL_H_
#define _CMATERIAL_H_

#include <vector>
#include <string>

#include "glm\vec3.hpp"

class CMaterial
{
public:
	/* Material Fields - fields to be passed to shader - can be several materials */
	std::string m_sName;
	float m_fNs;
	glm::vec3 m_vKa; //Ambient color (in RGB)
	glm::vec3 m_vKd; //Diffuse color (in RGB)
	glm::vec3 m_vKs; //Specular color (in RGB)
	float m_fNi;
	float m_fd;
	GLuint m_iIllum;
	std::string m_sMap_ka; //Texture ambient map
	std::string m_sMap_kd; //Texture diffuse map
	std::string m_sMap_ks; //Texture specular map

	GLuint m_iMap_ka_texture; //Texture id for Ambient
	GLuint m_iMap_ks_texture; //Texture id for Specular
	GLuint m_iMap_kd_texture; //Texture id for Diffuse

public:
	CMaterial(std::string file);
	~CMaterial();
};

#endif

//Special values from material file:
//Ns	Exponent specular light. Values from 0 to 1000
//Ni	Optical density -> from 0.001 to 10.0 value=1.0 Not blend value=1.5 glass
//d		Dissolve
//
//
//illum		Illumination    Properties that are turned on in the
//0		Color on and Ambient off
//1		Color on and Ambient on
//2		Highlight on
//3		Reflection on and Ray trace on
//4		Transparency: Glass on
//		 Reflection : Ray trace on
//5		Reflection : Fresnel on and Ray trace on
//6		Transparency : Refraction on
//		 Reflection : Fresnel off and Ray trace on
//7		Transparency : Refraction on
//		 Reflection : Fresnel on and Ray trace on
//8		Reflection on and Ray trace off
//9		Transparency : Glass on
//	     Reflection : Ray trace off
//10	Casts shadows onto invisible surfaces