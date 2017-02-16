// ImpEngineII.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cgameengine.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int loopRet = 0;
	CGameEngine *gameEngine = NULL;

	try
	{
		//Creates the Game Engine loaded by parameters file
		gameEngine = new CGameEngine("objetos\\engine.dat");

		//Enter the game engine loop
		loopRet = gameEngine->EnterMessageLoop();

		//Exit and delete the gameEngine object
		delete gameEngine;

		return loopRet;
	}
	catch (char *sz)
	{
		cerr << sz;

		delete gameEngine;
	}
	
	return 0;
}
