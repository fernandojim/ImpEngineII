#ifndef _CGAMEENGINE_H_
#define _CGAMEENGINE_H_

#include "cwopengl.h"
#include "ctimer.h"
#include "ccamera.h"
#include "cworld.h"

class CTimer;
class CCamera;
class CWorld;

class CGameEngine : public CWOpenGL 
{
private:
	/* Timer for real time gamming */
	CTimer  *m_gameTimer;

	/* First person camera */
	CCamera *m_gameCamera;

	/* Container of game objects */
	CWorld  *m_gameWorld;

	void GameCycle(double deltaTime);
	void OnPrepare();
	void CheckInput(double deltaTime);

	bool m_bQuit;

protected:
	void OnKeyDown(SDL_Keycode key);
	void OnMouseMove(int deltaX, int deltaY);

public:
	CGameEngine(const char *szName, bool fscreen, int w, int h, int b) :
		CWOpenGL(szName, fscreen, w, h, b)
	{
		m_bQuit = false;
		m_gameCamera = new CCamera();
		m_gameWorld  = new CWorld(m_gameCamera);
		m_gameCamera->centerX = 100;
		m_gameCamera->centerY = 100;
	}
	~CGameEngine() {}

	CCamera *OnGetCamera() { return m_gameCamera; }
	CWorld  *OnGetWorld()  { return m_gameWorld; }
	LRESULT EnterMessageLoop();
};

#endif 