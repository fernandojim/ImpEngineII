#ifndef _CWORLD_H_
#define _CWORLD_H_

#include "glm\vec3.hpp"
#include "cterrain.h"
#include "ccamera.h"
#include "cfont.h"
#include "cobject.h"
#include "cnode.h"
#include "cmesh.h"
#include "clight.h"
#include "globals.h"

class CCamera;
class CTerrain;
class CFont;
class CObject;

#define MAX_NODES 5 //Levels into hierarchy

using namespace GLOBALS;

class CWorld
{
public:
	//cámara asociada
	CCamera        *m_pCamara;

	//Luces
	CLight          m_Lights[2];

	//Malla del terreno
	CTerrain		*m_pTerrain;
	
	//Texto en OpenGL
	CFont           *m_pFont;

	//Hierarchical structure for objects
	std::vector<CNode*> m_pNodes;

public:
	CWorld(CCamera *c);
	~CWorld();

	void Update();
	void Animate(double dt);
	void Draw(CCamera *c);
};

#endif