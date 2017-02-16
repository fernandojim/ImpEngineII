#include "stdafx.h"

#include "cworld.h"

using namespace RenderManager;
using namespace GameObjectManager;

CWorld::CWorld(string sz_file)
{
	GLuint light = 0;
	//Creates the file to read info
	CFileDef *filedef = new CFileDef(sz_file);

	//Creamos las fuentes OpenGL
	m_pFont = new CFont("shaders\\fonts.vertex", "shaders\\fonts.fragment");

	if (filedef->getIsOpen())
	{
		//Read values from file
		filedef->readObjectKeysValues();

		//Crea luces
		// luz 0
		light = std::stoi(filedef->getObjectValues("LIGHT")[light]);

		m_Lights[light].m_light.lightPosition.x = std::stof(filedef->getObjectValues("POSITION")[0]);
		m_Lights[light].m_light.lightPosition.y = std::stof(filedef->getObjectValues("POSITION")[1]);
		m_Lights[light].m_light.lightPosition.z = std::stof(filedef->getObjectValues("POSITION")[2]);

		m_Lights[light].m_light.lightIntensity.x = std::stof(filedef->getObjectValues("INTENSITY")[0]);
		m_Lights[light].m_light.lightIntensity.y = std::stof(filedef->getObjectValues("INTENSITY")[1]);
		m_Lights[light].m_light.lightIntensity.z = std::stof(filedef->getObjectValues("INTENSITY")[2]);

		m_Lights[light].m_light.ka.x = std::stof(filedef->getObjectValues("KA")[0]);
		m_Lights[light].m_light.ka.y = std::stof(filedef->getObjectValues("KA")[1]);
		m_Lights[light].m_light.ka.z = std::stof(filedef->getObjectValues("KA")[2]);

		m_Lights[light].m_light.kd.x = std::stof(filedef->getObjectValues("KD")[0]);
		m_Lights[light].m_light.kd.y = std::stof(filedef->getObjectValues("KD")[1]);
		m_Lights[light].m_light.kd.z = std::stof(filedef->getObjectValues("KD")[2]);

		m_Lights[light].m_light.ks.x = std::stof(filedef->getObjectValues("KS")[0]);
		m_Lights[light].m_light.ks.y = std::stof(filedef->getObjectValues("KS")[1]);
		m_Lights[light].m_light.ks.z = std::stof(filedef->getObjectValues("KS")[2]);

		m_Lights[light].m_light.shininess = std::stof(filedef->getObjectValues("SHININESS")[0]);
	}

	//Establecemos la cámara de la escena
	m_pCamara = new CCamera(std::stoi(filedef->getObjectValues("CAM_ASPECT")[0]),  // width
							std::stoi(filedef->getObjectValues("CAM_ASPECT")[1]),  // height
							std::stof(filedef->getObjectValues("CAM_ASPECT")[2])); // aspect
	m_pCamara->position = glm::vec3(std::stoi(filedef->getObjectValues("CAM_POSITION")[0]), 0.0, std::stoi(filedef->getObjectValues("CAM_POSITION")[1]));

	//Create the manager of renderers
	getRenderManager().loadShadersFromFile(filedef->getObjectValues("SHADERS")[0]);

	//Create the main terrain
	getGameObjectManager().CreateGameObject(filedef->getObjectValues("TERRAIN")[0], OBJECT_TYPE::TERRAIN, OBJECT_HIERARCHY_LEVEL::LEVEL_0, 0, 0x0, m_pCamara, this, 0, 0, 0);

	getGameObjectManager().CreateGameObject("objetos\\Array House Example_obj.obj", OBJECT_TYPE::BLENDER, OBJECT_HIERARCHY_LEVEL::LEVEL_0, 0, 0x0, m_pCamara, this, 0, 0, 0);
	getGameObjectManager().CreateGameObject(filedef->getObjectValues("MD2")[1], OBJECT_TYPE::MD2, OBJECT_HIERARCHY_LEVEL::LEVEL_1, 0, 0x0, m_pCamara, this, 200, 100, 200);
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
	getRenderManager().Render();
}

CWorld::~CWorld()
{
	delete m_pCamara;
	delete m_pFont;
}
