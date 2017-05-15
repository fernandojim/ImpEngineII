#ifndef _CWOPENGL_H_
#define _CWOPENGL_H_

#include "stdafx.h"
#include "cfiledef.h"

#define OPENGL_VERSION_MAYOR 4
#define OPENGL_VERSION_MINOR 4

class CWOpenGL 
{
protected:
	SDL_Window   *m_mainwindow; /* Our window handle */
	SDL_GLContext m_maincontext; /* Our opengl context handle */

	bool m_arrbKeys[SDL_NUM_SCANCODES]; /* Array with the keys scancodes */

	/* Engine parameters file */
	CFileDef *m_File;

private:
	void Create();
	void Resize();
	void Finalize();

public:
	int m_width;
	int m_height;
	int m_bits;
	int m_aspect;
	int m_mouseX;
	int m_mouseY;
	bool m_bfullscreen;
	bool m_bvsync;
	string m_szName;
	string m_sWorldFile;

	float m_mouseSensitivity;

protected:
	virtual bool OnCreate() { return true; }
	virtual bool OnClose() { return true; }
	virtual void OnSize() { }
	virtual void OnMouseDownL(float x, float y) { }
	virtual void OnMouseDownR(float x, float y) { }
	virtual void OnMouseUpL() { }
	virtual void OnMouseUpR() { }
	virtual void OnMouseMove(int x, int y, int centerX, int centerY) { }
	virtual void OnMouseMove(int deltaX, int deltaY) { }
	virtual void OnMouseDragL(int x, int y, int dx, int dy) { }
	virtual void OnMouseDragR(int x, int y, int dx, int dy) { }
	virtual void OnKeyUp(SDL_Keycode key) { }
	virtual void OnKeyDown(SDL_Keycode key) { }
	virtual void OnChar(char c) { }

public:
	CWOpenGL(const char *szFile);
	CWOpenGL(const char *szName, bool vsync, bool fscreen, int w, int h, int b);
	virtual ~CWOpenGL() {}
};

#endif
