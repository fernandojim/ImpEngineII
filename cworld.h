
#ifndef _CWORLD_H_
#define _CWORLD_H_

#include "cgameobject.h"
#include "glm\vec3.hpp"
#include "cterrain.h"
#include "ccamera.h"
#include "cfont.h"
#include "cmesh.h"
#include "clight.h"
#include "cfiledef.h"

#include "rendermanager.h"
#include "gameobjectmanager.h"

class CCamera;
class CTerrain;
class CFont;
class CObject;

#define MAX_NODES 5 //Levels into hierarchy

class CWorld
{
public:
	//cámara asociada
	CCamera        *m_pCamara;

	//Luces
	CLight          m_Lights[2];

	//Texto en OpenGL
	CFont           *m_pFont;

public:
	CWorld(string sz_file);
	~CWorld();

	void Update();
	void Animate(double dt);
	void Draw(CCamera *c);
};

#endif
