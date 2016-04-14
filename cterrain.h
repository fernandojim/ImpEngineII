#ifndef _CTERRAIN_H_
#define _CTERRAIN_H_

#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm/gtc/type_ptr.hpp"	//Header for glm::value_ptr

#include "cbmp.h"
#include "ccamera.h"
#include "cworld.h"
#include "cfiledef.h"
#include "cobject.h"
#include "nvertex.h"
#include "clight.h"

#define NUM_TEXTURAS 4
#define SAND  1
#define GRASS 2
#define DIRT  3
#define SNOW  4

class CBmp;
class CShader;
class CVao;
class CFileDef;
class CWorld;

using namespace NVERTEX;

class CTerrain : public CObject
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

	bool addTexture(GLuint id);

private:
	//Bmp con el mapa DTED2 para las alturas
	CBmp	  *m_pHeightMap;
		
	CCamera	   *m_camera;

	CWorld     *m_world;

	//Geometric info
	GLuint      m_iNumVertex;
	GLuint		m_iNumIndex;
	Vertex3*    m_Vertex;
	Vertex2*    m_Texel;
	Vertex3*    m_Normal;
	GLuint*     m_Index;
		               
public:
	//Métodos
	CTerrain(const char *file);
    CTerrain(const char* vs, const char* fs, float ancho, float largo, float altura, float tilingfactor, char *sTexturaHeightfield);
    ~CTerrain();

	void     SetCamera(CCamera *c)  { m_camera = c; }
	CCamera* getCamera()		    { return m_camera; }

	void	 SetWorld(CWorld *w)    { m_world = w; }

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

	Vertex3*	getVertex();
	Vertex3*	getNormal();
	Vertex2*	getTexel();
	GLuint*     getIndices();
	GLuint		getNumVertex();
				
	float   CalculaAlturaMax();
	Vertex3 CalculaNormalVertice(GLuint iVertice);

	void Animate(double dt);
	void Update();
	void Render();
};

#endif
