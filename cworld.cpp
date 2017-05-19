#include "stdafx.h"

#include "cworld.h"

using namespace RenderManager;
using namespace GameObjectManager;

CWorld::CWorld(string sz_file)
{
	GLuint light = 0;
	//Creates the file to read info
	CFileDef *filedef = new CFileDef(sz_file);

	//Create Frame Buffer Object
	getGameObjectManager().m_fbo = new CFbo(512, 512);

	//Creamos las fuentes OpenGL
	m_pFont = new CFont("shaders\\fonts.vertex", "shaders\\fonts.fragment");

	if (filedef->getIsOpen())
	{
		//Read values from file
		filedef->readObjectKeysValues();

		//Lights
		// light 0
		light = filedef->getIntObjectValues("LIGHT")[light];

		m_Lights[light].m_light.lightPosition.x = filedef->getFloatObjectValues("POSITION")[0];
		m_Lights[light].m_light.lightPosition.y = filedef->getFloatObjectValues("POSITION")[1];
		m_Lights[light].m_light.lightPosition.z = filedef->getFloatObjectValues("POSITION")[2];

		m_Lights[light].m_light.lightIntensity.x = filedef->getFloatObjectValues("INTENSITY")[0];
		m_Lights[light].m_light.lightIntensity.y = filedef->getFloatObjectValues("INTENSITY")[1];
		m_Lights[light].m_light.lightIntensity.z = filedef->getFloatObjectValues("INTENSITY")[2];

		//Fog
		m_fog.fogColor.x = filedef->getFloatObjectValues("FOG_COLOR")[0];
		m_fog.fogColor.y = filedef->getFloatObjectValues("FOG_COLOR")[1];
		m_fog.fogColor.z = filedef->getFloatObjectValues("FOG_COLOR")[2];

		m_fog.minDist = filedef->getFloatObjectValues("DISTANCE")[0];
		m_fog.maxDist = filedef->getFloatObjectValues("DISTANCE")[1];
	}

	//Establecemos la cámara de la escena
	m_pCamara = new CCamera(filedef->getIntObjectValues("CAM_ASPECT")[0],  // width
							filedef->getIntObjectValues("CAM_ASPECT")[1],  // height
							filedef->getIntObjectValues("CAM_ASPECT")[2]); // aspect
	m_pCamara->position = glm::vec3(filedef->getIntObjectValues("CAM_POSITION")[0], 0.0, filedef->getIntObjectValues("CAM_POSITION")[1]);

	//Creates the Sky map as a cube map
	m_cubeMap = new CCubemap(filedef->getStringObjectValues("SKYBOX")[0]);

	//Create the manager of renderers
	getRenderManager().loadShadersFromFile(filedef->getStringObjectValues("SHADERS")[0]);

	//Create the main terrain
	getGameObjectManager().CreateGameObject(filedef->getStringObjectValues("TERRAIN")[0], OBJECT_TYPE::TERRAIN, OBJECT_HIERARCHY_LEVEL::LEVEL_0, 0, 0x0, m_pCamara, this, 0, 0, 0);

	for (int i=0;i<filedef->getCount("MESH");i++)
	{
		getGameObjectManager().CreateGameObject(filedef->getObjectValuesIndex("MESH", i)[1], OBJECT_TYPE::BLENDER, OBJECT_HIERARCHY_LEVEL::LEVEL_1, 0, 0x0, m_pCamara, this, (GLfloat)std::stof(filedef->getObjectValuesIndex("MESH", i)[2]), (GLfloat)std::stof(filedef->getObjectValuesIndex("MESH", i)[3]), (GLfloat)std::stof(filedef->getObjectValuesIndex("MESH", i)[4]));
	}

	for (int i=0;i<filedef->getCount("MD2");i++)
	{
		getGameObjectManager().CreateGameObject(filedef->getObjectValuesIndex("MD2", i)[1], OBJECT_TYPE::MD2, OBJECT_HIERARCHY_LEVEL::LEVEL_1, 0, 0x0, m_pCamara, this, (GLfloat)std::stof(filedef->getObjectValuesIndex("MD2", i)[2]), (GLfloat)std::stof(filedef->getObjectValuesIndex("MD2", i)[3]), (GLfloat)std::stof(filedef->getObjectValuesIndex("MD2", i)[4]));
	}
}

void CWorld::Animate(double dt)
{
	getGameObjectManager().AnimateGameObjects(dt);
}

void CWorld::Update()
{
	getGameObjectManager().UpdateGameObjects();
}

void CWorld::Draw(CCamera *c)
{
	m_cubeMap->Render(c);
	getRenderManager().Render();
}

CWorld::~CWorld()
{
	delete m_pCamara;
	delete m_pFont;
}
