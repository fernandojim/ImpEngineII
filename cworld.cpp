#include "stdafx.h"

#include "cworld.h"

CWorld::CWorld(CCamera *c)
{
	CNode *node0, *node1; //Hierarchy nodes
	CMesh *sub[2];        //Meshes for hierarchy level 2
	std::vector<CObject*>::iterator it; //itterator

	//Crea luces
	m_Lights[0].m_light.lightIntensity = glm::vec3(0.4, 0.4, 0.4);
	m_Lights[0].m_light.ka = glm::vec3(0.4, 0.4, 0.4);
	m_Lights[0].m_light.kd = glm::vec3(0.9, 0.9, 0.9);
	m_Lights[0].m_light.ks = glm::vec3(0.1, 0.1, 0.1);
	m_Lights[0].m_light.shininess = 1.0;
	m_Lights[0].m_light.lightPosition = glm::vec3(1000.0, 1000.0, 1000.0);

	//Establecemos la cámara de la escena
	m_pCamara = c;
	m_pCamara->position = glm::vec3(100.0, 200.0, 100.0);

	//Creamos el manager de texturas y las añadimos
	::getTextureManager().loadTexture("texturas\\sand.bmp");
	::getTextureManager().loadTexture("texturas\\grass1024.bmp");
	::getTextureManager().loadTexture("texturas\\dirt.bmp");
	::getTextureManager().loadTexture("texturas\\snow.bmp");
	::getTextureManager().loadTexture("texturas\\noise.bmp");
	GLuint texF = ::getTextureManager().loadTexture("texturas\\fonts_A_Z_0_9.bmp");
	
	//Creamos las fuentes OpenGL
	m_pFont = new CFont("shaders\\fonts.vertex", "shaders\\fonts.fragment");
	m_pFont->setIdTexture(texF);

	//Creamos la malla del terreno
	m_pTerrain = new CTerrain("objetos\\terrain.dat");
	m_pTerrain->SetCamera(m_pCamara);
	m_pTerrain->SetWorld(this);

	//Crea los nodos para cada nivel del árbol de objetos
	node0 = new CNode();
	m_pNodes.push_back(node0);

	//Inserta el terrain en el nivel 0
	it = m_pNodes.at(0)->m_Objects.begin();
	it = m_pNodes.at(0)->m_Objects.insert(it, m_pTerrain);

	sub[0] = new CMesh("House", "objetos\\avion.obj", "shaders\\mesh1.vertex", "shaders\\mesh1.fragment");
	sub[0]->m_position = glm::vec3(0.0, 0.0, 0.0);
	sub[0]->m_scale = glm::vec3(10.0, 10.0, 10.0);
	sub[0]->SetWorld(this);
	sub[0]->SetCamera(m_pCamara);
	sub[0]->m_pParent = m_pTerrain;

	/*sub[1] = new CMesh("tank1", "objetos\\tank.obj", "shaders\\mesh1.vertex", "shaders\\mesh1.fragment");
	sub[1]->m_position = glm::vec3(0.0, 100.0, 0.0);
	sub[1]->m_scale = glm::vec3(10.0, 10.0, 10.0);
	sub[1]->SetWorld(this);
	sub[1]->SetCamera(m_pCamara);
	sub[1]->m_pParent = m_pTerrain;*/

	//Inserta objetos en el nivel 1
	node1 = new CNode();
	m_pNodes.push_back(node1);
	it = m_pNodes.at(1)->m_Objects.begin();
	it = m_pNodes.at(1)->m_Objects.insert(it, sub[0]);
	//it = m_pNodes.at(1)->m_Objects.insert(it, sub[1]);
}

void CWorld::Animate(double dt)
{
	//Traverses the nodes list and Animate each one
	for (GLuint i = 0; i < m_pNodes.size(); i++)
		m_pNodes.at(i)->Animate(dt);
}

void CWorld::Update()
{
	//Traverses the nodes list and Update each one
	for (GLuint i = 0; i < m_pNodes.size(); i++)
		m_pNodes.at(i)->Update();
}

void CWorld::Draw(CCamera *c)
{
	//Traverses the nodes list and Draw each one
	for (GLuint i = 0; i < m_pNodes.size(); i++)
		m_pNodes.at(i)->Render();
}

CWorld::~CWorld()
{
	delete m_pCamara;
	delete m_pTerrain;
	delete m_pFont;
}
