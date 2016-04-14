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
#include "stdafx.h"

#include "cmaterial.h"

#include "cfiledef.h"
#include "globals.h"

using namespace GLOBALS;

#define SZ_NEWMTL sizeof("newmtl")-1
#define SZ_NS     sizeof("Ns")-1
#define SZ_KA     sizeof("Ka")-1
#define SZ_KD     sizeof("Kd")-1
#define SZ_KS     sizeof("Ks")-1
#define SZ_NI     sizeof("Ni")-1
#define SZ_D      sizeof("d")-1
#define SZ_ILLUM  sizeof("illum")-1
#define SZ_MAPKD  sizeof("map_Kd")-1
#define SZ_MAPKS  sizeof("map_Ks")-1
#define SZ_MAPKA  sizeof("map_Ka")-1

using namespace std;

CMaterial::CMaterial(std::string file)
{
	std::string aux[4];
	std::string line;
	CFileDef *filedef = new CFileDef("objetos\\"+file);
	GLuint mat = 0;

	/* if exist */
	if (filedef && filedef->getIsOpen())
	{
		while (!filedef->m_filedef.eof())
		{
			/* get line from file and stores into buffer */
			std::getline(filedef->m_filedef, line);

			/* ignores '#' '\n' '{' or '\0' characters */
			if (line.size() != 0 && line.at(0) != '#' && line.at(0) != '\n' && line.at(0) != '\0')
			{
				//Get the field newmtl
				if (!line.compare(0, SZ_NEWMTL, "newmtl"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					std::stringstream(aux[1]) >> m_sName;
				}
				else if (!line.compare(0, SZ_NS, "Ns"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_fNs = std::stof(aux[1]);
				}
				else if (!line.compare(0, SZ_KA, "Ka"))
				{
					std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
					m_vKa.x = std::stof(aux[1]);
					m_vKa.y = std::stof(aux[2]);
					m_vKa.z = std::stof(aux[3]);
				}
				else if (!line.compare(0, SZ_KD, "Kd"))
				{
					std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
					m_vKd.x = std::stof(aux[1]);
					m_vKd.y = std::stof(aux[2]);
					m_vKd.z = std::stof(aux[3]);
				}
				else if (!line.compare(0, SZ_KS, "Ks"))
				{
					std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
					m_vKs.x = std::stof(aux[1]);
					m_vKs.y = std::stof(aux[2]);
					m_vKs.z = std::stof(aux[3]);
				}
				else if (!line.compare(0, SZ_NI, "Ni"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_fNi = std::stof(aux[1]);
				}
				else if (!line.compare(0, SZ_D, "d"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_fd = std::stof(aux[1]);
				}
				else if (!line.compare(0, SZ_ILLUM, "illum"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_iIllum = std::stoi(aux[1]);
				}
				else if (!line.compare(0, SZ_MAPKA, "map_Ka"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_sMap_ka = aux[1];
					m_iMap_ka_texture = ::getTextureManager().loadTexture(m_sMap_ka);
				}
				else if (!line.compare(0, SZ_MAPKD, "map_Kd"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_sMap_kd = aux[1];
					m_iMap_kd_texture = ::getTextureManager().loadTexture(m_sMap_kd);
				}
				else if (!line.compare(0, SZ_MAPKS, "map_Ks"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_sMap_ks = aux[1];
					m_iMap_ks_texture = ::getTextureManager().loadTexture(m_sMap_ks);
				}
			}
		}
	}
}

CMaterial::~CMaterial()
{

}