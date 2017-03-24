/*
 * gameobjectmanager.h
 *
 *  Created on: 26 de oct. de 2016
 *      Author: fjimartinez
 */

#ifndef GAMEOBJECTMANAGER_H_
#define GAMEOBJECTMANAGER_H_

#include "stdafx.h"

#include <iostream>
#include <vector>

#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\euler_angles.hpp"
#include "cterrain.h"
#include "cmesh.h"
#include "cmd2model.h"

using namespace std;

class CTerrain;
class CMesh;
class CMd2model;

namespace GameObjectManager
{
	class CGameObjectManager
	{
		private:
			GLuint m_posGameObject;

		public:
			//Objects lists
			// List of terrains
			std::vector < CTerrain > m_GameObjectTerrain;

			// List of Meshes (Blender3D objects)
			std::vector < CMesh > m_GameObjectsMesh;

			// List of MD2 models
			std::vector < CMd2model > m_GameObjectsMD2;

		public:
			CGameObjectManager();
			~CGameObjectManager();

			//Creates an object
			CGameObject* CreateGameObject(const string sObj, OBJECT_TYPE tp, OBJECT_HIERARCHY_LEVEL lvl, GLuint id, CGameObject *parent, CCamera *cam, CWorld *w, GLfloat x, GLfloat y, GLfloat z);

			//Update the objects
			void UpdateGameObjects();

			//Animate the objects
			void AnimateGameObjects(double dt);
	};

	// Get the instance of gameobjectmanager
	CGameObjectManager& getGameObjectManager();
};


#endif /* GAMEOBJECTMANAGER_H_ */
