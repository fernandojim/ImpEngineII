/*
 * materialmanager.cpp
 *
 *  Created on: 26 de oct. de 2016
 *      Author: fjimartinez
 */

#include "materialmanager.h"

using namespace MaterialManager;
using namespace TextureManager;

#define SZ_NEWMTL  sizeof("newmtl")-1
#define SZ_NS      sizeof("Ns")-1
#define SZ_KA      sizeof("Ka")-1
#define SZ_KD      sizeof("Kd")-1
#define SZ_KS      sizeof("Ks")-1
#define SZ_NI      sizeof("Ni")-1
#define SZ_D       sizeof("d")-1
#define SZ_ILLUM   sizeof("illum")-1
#define SZ_MAPKD   sizeof("map_Kd")-1
#define SZ_MAPKS   sizeof("map_Ks")-1
#define SZ_MAPKA   sizeof("map_Ka")-1
#define SZ_MAPBUMP sizeof("map_Bump")-1

/*
	Get the material manager
*/
CMaterialManager& MaterialManager::getMaterialManager()
{
	static CMaterialManager tm;

	return tm;
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

	m_Materials.clear();
}

void CMaterialManager::loadMaterialsFromMTL(const string sMTLName)
{
	CMaterial* pMaterial = NULL;
	string sid;
	std::map < string, CMaterial* >::iterator iter;

	string mat_name;
	string aux[4];
	string line;
	bool found = false;

	//Load Materials from file mtl
	CFileDef *filedef = new CFileDef(sMTLName);

	//Test if this material exists in the library of materials
	pMaterial = getMaterialByName(sMTLName);

	//Load material
	if (pMaterial == NULL && filedef->getIsOpen())
	{
		while (!filedef->m_Filedef.eof())
		{
			/* get line from file and stores into buffer */
			std::getline(filedef->m_Filedef, line);

			/* ignores '#' '\n' '{' or '\0' characters */
			if (line.size() != 0 && line.at(0) != '#' && line.at(0) != '\n' && line.at(0) != '\0')
			{
				//Get the field newmtl
				if (!line.compare(0, SZ_NEWMTL, "newmtl"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					std::stringstream(aux[1]) >> mat_name;

					//Test if the material already exist
					for (iter = m_Materials.begin(); iter != m_Materials.end(); ++iter)
					{
						string name = iter->first;

						if (!name.compare(mat_name))
						{
							found = true;
							break;
						}
					}

					//The material does not exists. Then, it is created and stored
					if ( !found )
					{
						pMaterial = new CMaterial();

						pMaterial->m_sName = mat_name;

						while (!filedef->m_Filedef.eof())
						{
							/* get line from file and stores into buffer */
							std::getline(filedef->m_Filedef, line);

							if (!line.compare(0, SZ_NS, "Ns"))
							{
								std::stringstream(line) >> aux[0] >> aux[1];
								pMaterial->m_fNs = std::stof(aux[1]);
							}
							else if (!line.compare(0, SZ_KA, "Ka"))
							{
								std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
								pMaterial->m_vKa.x = std::stof(aux[1]);
								pMaterial->m_vKa.y = std::stof(aux[2]);
								pMaterial->m_vKa.z = std::stof(aux[3]);
							}
							else if (!line.compare(0, SZ_KD, "Kd"))
							{
								std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
								pMaterial->m_vKd.x = std::stof(aux[1]);
								pMaterial->m_vKd.y = std::stof(aux[2]);
								pMaterial->m_vKd.z = std::stof(aux[3]);
							}
							else if (!line.compare(0, SZ_KS, "Ks"))
							{
								std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
								pMaterial->m_vKs.x = std::stof(aux[1]);
								pMaterial->m_vKs.y = std::stof(aux[2]);
								pMaterial->m_vKs.z = std::stof(aux[3]);
							}
							else if (!line.compare(0, SZ_NI, "Ni"))
							{
								std::stringstream(line) >> aux[0] >> aux[1];
								pMaterial->m_fNi = std::stof(aux[1]);
							}
							else if (!line.compare(0, SZ_D, "d"))
							{
								std::stringstream(line) >> aux[0] >> aux[1];
								pMaterial->m_fd = std::stof(aux[1]);
							}
							else if (!line.compare(0, SZ_ILLUM, "illum"))
							{
								std::stringstream(line) >> aux[0] >> aux[1];
								pMaterial->m_iIllum = std::stoi(aux[1]);
							}
							else if (!line.compare(0, SZ_MAPBUMP, "map_Bump"))
							{
								pMaterial->m_sMap_Bump = line.substr(9, line.length());
								pMaterial->m_iMap_Bump_texture = ::getTextureManager().loadTexture(pMaterial->m_sMap_Bump);
							}
							else if (!line.compare(0, SZ_MAPKA, "map_Ka"))
							{
								pMaterial->m_sMap_ka = line.substr(7, line.length());
								pMaterial->m_iMap_ka_texture = ::getTextureManager().loadTexture(pMaterial->m_sMap_ka);
							}
							else if (!line.compare(0, SZ_MAPKD, "map_Kd"))
							{
								pMaterial->m_sMap_kd = line.substr(7, line.length());
								pMaterial->m_iMap_kd_texture = ::getTextureManager().loadTexture(pMaterial->m_sMap_kd);
							}
							else if (!line.compare(0, SZ_MAPKS, "map_Ks"))
							{
								pMaterial->m_sMap_ks = line.substr(7, line.length());
								pMaterial->m_iMap_ks_texture = ::getTextureManager().loadTexture(pMaterial->m_sMap_ks);
							}
							//... Other values
						}

						//Add the new material
						m_Materials.insert(std::pair < string, CMaterial* >(mat_name, pMaterial));
					}
				}
			}
		}

		//Close file
		delete filedef;
	}
}

CMaterial* CMaterialManager::getMaterialByName(const string sName)
{
	std::map < string, CMaterial* >::iterator iter;

	//Gets the map of id
	iter = m_Materials.find(sName);

	if (iter != m_Materials.end())
		return (CMaterial*)iter->second;
	else
		return NULL;
}


