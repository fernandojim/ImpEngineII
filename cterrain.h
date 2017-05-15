#ifndef _CTERRAIN_H_
#define _CTERRAIN_H_

#include <string>

#include "cgameobject.h"
#include "cmaterial.h"
#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm/gtc/type_ptr.hpp"	//Header for glm::value_ptr

#include "cbmp.h"
#include "ccamera.h"
#include "cworld.h"
#include "cfiledef.h"
#include "nvertex.h"
#include "clight.h"

using namespace NVERTEX;

#define MAX_TEXTURES 5

class CTerrain : public CGameObject
{
public:
	string m_vs;
	string m_fs;
	GLuint m_iDivisiones;
	float  m_fAncho;
	float  m_fLargo;
	float  m_fAlturaMax;
	float  m_fTilingFactor;
	string m_texturaHeightfield;

	CFileDef *m_File;

	//Bmp con el mapa DTED2 para las alturas
	CBmp	  *m_pHeightMap;

	bool addTexture(GLuint id);

public:
	CMaterial 	m_MatSand;
	CMaterial 	m_MatGrass;
	CMaterial 	m_MatDirt;
	CMaterial 	m_MatSnow;

	//Tiling texture factors
	GLfloat		m_tilingFactorSand;
	GLfloat		m_tilingFactorGrass;
	GLfloat		m_tilingFactorDirt;
	GLfloat		m_tilingFactorSnow;

	//Materials - one material per texture
	std::string m_materialsName;

public:
	CTerrain(const string file);
    ~CTerrain();

	void    setLights(CLight *lights);

	void    obtieneIndicesTriangulo(glm::vec3 pos, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);
    float   GetAltura(float x, float z);    //Dando las coordenadas (x, z), obtiene la coordenada y
	GLuint  getIndice(GLuint x, GLuint z);          //Obtenemos el indice del vértice dando el vértice (x,z)
	void    setAltura(GLuint indice, float altura);           //Damos altura al vértice referenciado por 'indice'
	bool    puntoInterior(GLuint ind);
	glm::vec3 calculaNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 getNormalAtPos(glm::vec3 pos);
	float   getFronteraX();
	float   getFronteraZ();
	float   CalculaAlturaMax();
	glm::vec3 CalculaNormalVertice(GLuint iVertice);

	void Animate(double dt);
	void Update();
};

#endif
