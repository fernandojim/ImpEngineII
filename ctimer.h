//////////////////////////////////////////////////////////////////////////////
//
//  --- CTimer.h ---
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __CTIMER_H__
#define __CTIMER_H__

#include <string>

class CTimer
{
public: 
	//Initializes variables 
	CTimer(); 
	
	//The various clock actions 
	void start(); 
	void stop(); 
	void pause(); 
	void unpause(); 
	
	//Gets ticks (millisecs) since start
	Uint32 getTicks();

	//Gets the fps - int
	int igetFPS();
	std::string sgetFPS();

	//Get elapsed seconds
	double getElapsedSeconds();

	//Checks the status of the timer 
	bool isStarted(); 
	bool isPaused(); 

private: 
	//The clock time when the timer started 
	Uint32 m_iStartTicks;
	
	//The ticks stored when the timer was paused 
	Uint32 m_iPausedTicks;

	//Frames per second
	int m_iFps;

	//The timer status 
	bool m_bPaused; 
	bool m_bStarted; 
};

#endif // __CTIMER_H___
