#include "stdafx.h"

#include <iostream>

#include "cwopengl.h"

CWOpenGL::CWOpenGL(const char *szName, bool fscreen, int w, int h, int b)
{
	m_width = w;
	m_height = h;
	m_bits = b;
	m_mouseX = 0;
	m_mouseY = 0;
	m_szName = (char*) szName;
	m_mouseSensitivity = 0.2f;

	Uint32 flags = SDL_WINDOW_OPENGL;

	/* Set full screen if fsscreen parameter is true */
	if (fscreen)
		flags = flags | SDL_WINDOW_FULLSCREEN;

	/* Initialize SDL's Video subsystem */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Unable to initialize SDL";
		return;
	}

	/* Request opengl 3.3 context. */
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAYOR);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);

	/* Turn on double buffering with a 24bit Z buffer.
	* You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_bits);

	/* Create our window centered at 512x512 resolution */
	m_mainwindow = SDL_CreateWindow(m_szName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_width, m_height, flags);

	/* Die if creation failed */
	if (!m_mainwindow)
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	/* Initialize the array of keys */
	memset(&m_arrbKeys, 0, sizeof(bool)*SDL_NUM_SCANCODES);
	
	/* Hidde cursor mouse and set it to window center */
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(m_mainwindow, 320, 240);

	/* Create our opengl context and attach it to our window */
	m_maincontext = SDL_GL_CreateContext(m_mainwindow);

	/* Load OpenGL functions */
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		exit(0);
	}

	/* This makes our buffer swap syncronized with the monitor's vertical refresh. Parameter: */
	/* 0 -> without sync refresh */
	/* 1 -> Sync refresh */
	SDL_GL_SetSwapInterval(0);
}

void CWOpenGL::Resize()
{
	if (m_width > m_height)
	{
		m_aspect = m_width;
	}
	else
	{
		m_aspect = m_height;
	}
	
	if (m_maincontext)
	{
		glViewport(0, 0, m_width, m_height);

		OnSize();
	}
}

void CWOpenGL::Finalize()
{
	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DeleteContext(m_maincontext);
	SDL_DestroyWindow(m_mainwindow);
	SDL_Quit();
}
