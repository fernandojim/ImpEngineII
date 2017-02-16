#include "stdafx.h"

#include "cgameengine.h"
#include "gameobjectmanager.h"

using namespace GameObjectManager;

void CGameEngine::Clear()
{
	m_gameTimer = NULL;
	m_gameCamera = NULL;
	m_gameWorld = NULL;
	m_bQuit = false;

	m_gameWorld  = new CWorld(m_sWorldFile);
	m_gameCamera = m_gameWorld->m_pCamara;
}

void CGameEngine::OnPrepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void CGameEngine::CheckInput(double deltaTime)
{
	static double buttonDelta = 0.0;

	buttonDelta -= deltaTime;

	if (buttonDelta < 0.0)
		buttonDelta = 0.0;

	/* Get the keys pressed */

	/* Keys for move the player */
	if (m_arrbKeys[SDL_SCANCODE_W])
		OnKeyDown(SDLK_w);

	if (m_arrbKeys[SDL_SCANCODE_S])
		OnKeyDown(SDLK_s);

	if (m_arrbKeys[SDL_SCANCODE_A])
		OnKeyDown(SDLK_a);

	if (m_arrbKeys[SDL_SCANCODE_D])
		OnKeyDown(SDLK_d);

	/* Keys for move the entity */
	if (m_arrbKeys[SDL_SCANCODE_T])
		OnKeyDown(SDLK_t);
	
	if (m_arrbKeys[SDL_SCANCODE_G])
		OnKeyDown(SDLK_g);

	if (m_arrbKeys[SDL_SCANCODE_F])
		OnKeyDown(SDLK_f);

	if (m_arrbKeys[SDL_SCANCODE_H])
		OnKeyDown(SDLK_h);

	/* Exit */
	if (m_arrbKeys[SDL_SCANCODE_ESCAPE])
		OnKeyDown(SDLK_ESCAPE);
}

void CGameEngine::OnKeyDown(SDL_Keycode key)
{
	// Adelante
	if (key == SDLK_w)
	{
		if (glm::length(m_gameCamera->velocity) < 25.0)
			m_gameCamera->velocity += glm::vec3(0.0, 0.0, 2.0);
	}

	// Atrás
	if (key == SDLK_s)
	{
		if (glm::length(m_gameCamera->velocity) < 25.0)
			m_gameCamera->velocity += glm::vec3(0.0, 0.0, -2.0);
	}

	// Izquierda
	if (key == SDLK_a)
	{
		if (glm::length(m_gameCamera->velocity) < 25.0)
			m_gameCamera->velocity += glm::vec3(-2.0, 0.0, 0.0);
	}

	// Derecha
	if (key == SDLK_d)
	{
		if (glm::length(m_gameCamera->velocity) < 25.0)
			m_gameCamera->velocity += glm::vec3(2.0, 0.0, 0.0);
	}

	if (key == SDLK_t)
	{
		::getGameObjectManager().m_GameObjectsMD2.at(0).m_velocity = glm::vec3(16.0, 16.0, 16.0);
	}

	if (key == SDLK_g)
	{

	}

	if (key == SDLK_f)
	{
		::getGameObjectManager().m_GameObjectsMD2.at(0).m_angle += 0.1;
	}

	if (key == SDLK_h)
	{
		::getGameObjectManager().m_GameObjectsMD2.at(0).m_angle -= 0.1;
	}

	// Tecla ESC
	if (key == SDLK_ESCAPE)
	{
		m_bQuit = true;
	}
}

void CGameEngine::OnMouseMove(int deltaX, int deltaY)
{
	m_gameCamera->yaw += deltaX * m_mouseSensitivity;
	m_gameCamera->pitch -= deltaY * m_mouseSensitivity;
}

void CGameEngine::GameCycle(double deltaTime)
{
	//setup opengl for frame (clear, identity)
	// Depth test, cull face...
	OnPrepare();

	/* Manage the input events */
	CheckInput(deltaTime);

	// move/orient camera
	m_gameCamera->Animate(deltaTime);

	// update world
	m_gameWorld->Update();

	// move/orient objects
	m_gameWorld->Animate(deltaTime);

	// draw objects
	m_gameWorld->Draw(m_gameCamera);

	// Draw the Frames Per Second
	m_gameWorld->m_pFont->Render(m_gameTimer->sgetFPS() +" Fps"+ "<" + (char*)glGetString(GL_VERSION) + "<" + (char*)glGetString(GL_VENDOR));

	//swap buffers (SDL2)
	SDL_GL_SwapWindow(m_mainwindow);
}

// EnterMessageLoop()
// desc: the Windows message loop
LRESULT CGameEngine::EnterMessageLoop()
{
	SDL_Event event;
	
	/* Crea el temporizador */
	m_gameTimer = new CTimer();

	/* Lo inicializa */
	m_gameTimer->start();
	
	/* Bucle principal */
	while (!m_bQuit)
	{
		/* Obtiene eventos de la cola en caso de que los hubiera */
		if (SDL_PollEvent(&event))
		{
			/* Los gestiona */
			switch (event.type)
			{
				case SDL_KEYDOWN:
					/* Enables the scancode of key pressed */
					m_arrbKeys[event.key.keysym.scancode] = true;
				break;

				case SDL_KEYUP:
					/* Disables the scancode of key pressed */
					m_arrbKeys[event.key.keysym.scancode] = false;
				break;

				case SDL_MOUSEMOTION:
					SDL_GetRelativeMouseState(&m_mouseX, &m_mouseY);
					OnMouseMove(m_mouseX, m_mouseY);
				break;
			}
		}
		else
		{
			/* Si no hay eventos en la cola Llama a loop principal del juego */
			GameCycle(m_gameTimer->getElapsedSeconds());
		}
	}

	delete m_gameTimer;

	return -1;
}
