#include "cwopengl.h"
#include "utils.h"

using namespace Message;

CWOpenGL::CWOpenGL(const char *szFile)
{
	string aux;

	//Create the file
	m_File = new CFileDef(szFile);

	// Read the content of file
	if (m_File->getIsOpen())
	{
		m_File->readObjectKeysValues();

		// Gets the name ob object
		m_szName = m_File->getObjectName();

		// Gets the properties from file */

		// FULLSCREEN
		aux = m_File->getStringObjectValues("FULLSCREEN")[0];
		if (aux == "N")
			m_bfullscreen = false;
		else if (aux == "Y")
			m_bfullscreen = true;
		else
		{
			MessageBOX("Engine file error", "Error near to 'FULLSCREEN'");
			exit(0);
		}

		// VERTICAL SYNC
		aux = m_File->getStringObjectValues("VSYNC")[0];
		if (aux == "N")
			m_bvsync = false;
		else if (aux == "Y")
			m_bvsync = true;
		else
		{
			MessageBOX("Error near to 'VSYNC'", "Engine file error");
			delete m_File;
			exit(0);
		}

		// WINDOW WIDTH
		m_width = m_File->getIntObjectValues("WINDOW_RES")[0];
		if (m_width < 320 || m_width > 1920)
		{
			MessageBOX("Window width value invalid", "Engine file error");
			delete m_File;
			exit(0);
		}

		// WINDOW HEIGHT
		m_height = m_File->getIntObjectValues("WINDOW_RES")[1];
		if (m_height < 200 || m_height > 1200)
		{
			MessageBOX("Window height value invalid", "Engine file error");
			delete m_File;
			exit(0);
		}

		// WINDOW DEPTH BITS
		m_bits = m_File->getIntObjectValues("WINDOW_RES")[2];
		if (m_bits != 8 && m_bits != 16 && m_bits != 24 && m_bits != 32)
		{
			MessageBOX("Window bits depth color value invalid", "Engine file error");
			delete m_File;
			exit(0);
		}

		// WORLD file
		m_sWorldFile = m_File->getStringObjectValues("WORLD")[0];
		if (m_sWorldFile == "")
		{
			MessageBOX("World file not error", "Engine file error");
			delete m_File;
			exit(0);
		}

		delete m_File;

		Create();
	}
	else
		MessageBOX("It does not exists", "Engine file error");

}

CWOpenGL::CWOpenGL(const char *szName, bool fscreen, bool vsync, int w, int h, int b)
{
	m_width = w;
	m_height = h;
	m_bits = b;
	m_bfullscreen = fscreen;
	m_bvsync = vsync;
	m_szName = (char*) szName;

	Create();
}

void CWOpenGL::Create()
{
	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseSensitivity = 0.2f;
	m_aspect = 0.0;

	Uint32 flags = SDL_WINDOW_OPENGL;

	/* Set full screen if fsscreen parameter is true */
	if (m_bfullscreen)
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
	m_mainwindow = SDL_CreateWindow(m_szName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
		throw std::exception();
	}

	//Initialize the scene
	InitScene();

	/*char cad[300];
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *language = glGetString(GL_SHADING_LANGUAGE_VERSION);

	sprintf(cad, "%s\n%s\n%s\n%s", vendor, renderer, version, language);
	MessageBOX("OpenGL info", cad);*/

	/* This makes our buffer swap syncronized with the monitor's vertical refresh. Parameter: */
	/* 0 -> without sync refresh */
	/* 1 -> Sync refresh */
	SDL_GL_SetSwapInterval((int)m_bvsync);
}

void CWOpenGL::InitScene()
{
	glViewport(0, 0, m_width, m_height);

	//Init OpenGL
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(GL_TRUE);
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
