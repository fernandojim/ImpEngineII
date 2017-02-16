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
	void Clear();

	bool m_bQuit;

protected:
	void OnKeyDown(SDL_Keycode key);
	void OnMouseMove(int deltaX, int deltaY);

public:

	/* Constructor by file */
	CGameEngine(const char *szfile) : CWOpenGL(szfile)
	{
		Clear();
	}

	CGameEngine(const char *szName, bool fscreen, bool vsync, int w, int h, int b) :
		CWOpenGL(szName, fscreen, vsync, w, h, b)
	{
		Clear();
	}

	/* Destructor */
	~CGameEngine() {}

	CCamera *OnGetCamera() { return m_gameCamera; }
	CWorld  *OnGetWorld()  { return m_gameWorld; }
	LRESULT EnterMessageLoop();
};

#endif 
