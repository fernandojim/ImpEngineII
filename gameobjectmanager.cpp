/*
 * gameobjectmanager.cpp
 *
 *  Created on: 26 de oct. de 2016
 *      Author: fjimartinez
 */

#include "gameobjectmanager.h"

using namespace GameObjectManager;

/*
	Get the object manager
*/
CGameObjectManager& GameObjectManager::getGameObjectManager()
{
	static CGameObjectManager tm;

	return tm;
}

/*
    ------------------------------
	Methods for CGameObjectManager
	------------------------------
*/
CGameObjectManager::CGameObjectManager()
{
	m_posGameObject = 0;
}

CGameObjectManager::~CGameObjectManager()
{

}

CGameObject* CGameObjectManager::CreateGameObject(const string sObj, OBJECT_TYPE tp, OBJECT_HIERARCHY_LEVEL lvl, GLuint id, CGameObject *parent, CCamera *cam, CWorld *w, GLfloat x, GLfloat y, GLfloat z)
{
	CGameObject *gameObject;

	//Instanciates the object according to type
	switch (tp)
	{
		case TERRAIN:
			gameObject = new CTerrain(sObj);
		break;

		case BLENDER:
			gameObject = new CMesh(sObj);
		break;

		case MD2:
			gameObject = new CMd2model(sObj);
		break;

		default:
			return NULL;
		break;
	}

	//test if object created is valid and proceed to asignate values
	if (gameObject->m_bIsValid)
	{
		gameObject->m_iId = id;
		gameObject->m_hierarchyLevel = lvl;
		gameObject->m_type = tp;
		gameObject->m_pParent = parent;
		gameObject->SetCamera(cam);
		gameObject->SetWorld(w);

		//Add object to proper object list according its level and type
		switch(tp)
		{
			case TERRAIN:
				m_GameObjectTerrain.push_back(*(CTerrain*)gameObject);
			break;

			case BLENDER:
				gameObject->m_position = glm::vec3(x, y, z);
				gameObject->m_scale = glm::vec3(5.0, 5.0, 5.0);
				m_GameObjectsMesh.push_back(*(CMesh*)gameObject);
			break;

			case MD2:
				gameObject->m_position = glm::vec3(x, y, z);
				gameObject->m_scale = glm::vec3(0.14, 0.14, 0.14);
				m_GameObjectsMD2.push_back(*(CMd2model*)gameObject);
			break;

			default:
				return NULL;
			break;
		}

		return gameObject;
	}
	else
		return NULL;
}

void CGameObjectManager::UpdateGameObjects()
{
	m_GameObjectTerrain[0].m_WorldTransformation = &m_GameObjectTerrain[0].m_camera->m_M4MVP;

	for ( auto &i : m_GameObjectsMesh )
	{
		CMesh *mesh = &i;

		//Local transformation
		*mesh->m_LocalTransformation = glm::mat4(1.0);
		*mesh->m_LocalTransformation = glm::translate(*mesh->m_LocalTransformation, mesh->m_position);
		*mesh->m_LocalTransformation *= glm::toMat4(mesh->m_qrotTo);
		*mesh->m_LocalTransformation = glm::scale(*mesh->m_LocalTransformation, mesh->m_scale);

		//World transformation
		*mesh->m_WorldTransformation = *m_GameObjectTerrain[0].m_WorldTransformation;
		*mesh->m_LocalTransformation = *mesh->m_WorldTransformation * (*mesh->m_LocalTransformation);
		*mesh->m_WorldTransformation = *mesh->m_LocalTransformation;
	}

	for ( auto &i : m_GameObjectsMD2 )
	{
		CMd2model *md2 = &i;

		//Update quaternion rotation
		md2->m_angles = glm::vec3(0.0, md2->m_angle , 0.0);
		md2->m_qrot = glm::quat(md2->m_angles);

		md2->m_facing.x = glm::sin(md2->m_angle + glm::radians(90.0));
		md2->m_facing.z = glm::cos(md2->m_angle + glm::radians(90.0));
		glm::normalize(md2->m_facing);

		md2->m_velocity.x = md2->m_facing.x * md2->m_speed;
		md2->m_velocity.z = md2->m_facing.z * md2->m_speed;

		//Colocates the md2 model object in the terrain level
		md2->m_position.y = m_GameObjectTerrain[0].GetAltura(md2->m_position.x, md2->m_position.z);

		//Local transformation
		*md2->m_LocalTransformation = glm::mat4(1.0); //Initialize local transformations
		*md2->m_LocalTransformation = glm::translate(*md2->m_LocalTransformation, md2->m_position);
		*md2->m_LocalTransformation *= glm::toMat4(md2->m_qrot);
		*md2->m_LocalTransformation = glm::scale(*md2->m_LocalTransformation, md2->m_scale);

		//World transformation (terrain)
		*md2->m_WorldTransformation = *m_GameObjectTerrain[0].m_WorldTransformation;
		*md2->m_LocalTransformation = *md2->m_WorldTransformation * (*md2->m_LocalTransformation);
		*md2->m_WorldTransformation = *md2->m_LocalTransformation;
	}
}

void CGameObjectManager::AnimateGameObjects(double dt)
{
	//Animate the camera
	m_GameObjectTerrain[0].m_camera->position.y = m_GameObjectTerrain[0].GetAltura(m_GameObjectTerrain[0].m_camera->position.x, m_GameObjectTerrain[0].m_camera->position.z);
	m_GameObjectTerrain[0].m_camera->position.y += 5.0;

	//Animate meshes
	for ( auto &i : m_GameObjectsMesh )
	{
		CMesh *mesh = &i;

		//Colocates the mesh object in the terrain level
		mesh->m_position.y = m_GameObjectTerrain[0].GetAltura(mesh->m_position.x, mesh->m_position.z);

		glm::vec3 norm = m_GameObjectTerrain[0].getNormalAtPos(mesh->m_position);
		mesh->m_angles = glm::vec3(norm.x, norm.y, norm.z);
		mesh->m_qrot = glm::quat(mesh->m_angles);

	}

	//Animate md2 models
	for ( auto &i : m_GameObjectsMD2 )
	{
		CMd2model *md2 = &i;

		//Update the md2 model animation by frames and deltaTime
		md2->UpdateAnimation(dt);
	}
}
