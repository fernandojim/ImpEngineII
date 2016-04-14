//////////////////////////////////////////////////////////////////////////////
//
//  --- ctimer.cpp ---
//
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ctimer.h"

CTimer::CTimer() 
{ 
	//Initialize the variables 
	m_iStartTicks = 0; 
	m_iPausedTicks = 0; 
	m_bPaused = false; 
	m_bStarted = false;
	m_iFps = 0;
}

void CTimer::start() 
{ 
	//Start the timer 
	m_bStarted = true; 
	
	//Unpause the timer 
	m_bPaused = false; 
	
	//Get the current clock time 
	m_iStartTicks = SDL_GetTicks(); 
	m_iPausedTicks = 0; 
}

void CTimer::stop() 
{ 
	//Stop the timer 
	m_bStarted = false; 
	
	//Unpause the timer 
	m_bPaused = false; 
	
	//Clear tick variables 
	m_iStartTicks = 0; 
	m_iPausedTicks = 0; 
}

void CTimer::pause() 
{ 
	//If the timer is running and isn't already paused 
	if( m_bStarted && !m_bPaused ) 
	{ 
		//Pause the timer 
		m_bPaused = true; 
		
		//Calculate the paused ticks 
		m_iPausedTicks = SDL_GetTicks() - m_iStartTicks; 
		m_iStartTicks = 0; 
	} 
}

void CTimer::unpause() 
{ 
	//If the timer is running and paused 
	if( m_bStarted && m_bPaused ) 
	{ 
		//Unpause the timer 
		m_bPaused = false; 
		
		//Reset the starting ticks 
		m_iStartTicks = SDL_GetTicks() - m_iPausedTicks; 
		
		//Reset the paused ticks 
		m_iPausedTicks = 0; 
	} 
}

Uint32 CTimer::getTicks() 
{ 
	//The actual timer time 
	Uint32 time = 0; 
	
	//If the timer is running 
	if( m_bStarted ) 
	{ 
		//If the timer is paused 
		if( m_bPaused ) 
		{ 
			//Return the number of ticks when the timer was paused 
			time = m_iPausedTicks; 
		} 
		else 
		{ 
			//Return the current time minus the start time 
			time = SDL_GetTicks() - m_iStartTicks; 
		} 
	} 
	
	return time; 
}

int CTimer::igetFPS()
{
	return m_iFps;
}

std::string CTimer::sgetFPS()
{
	return std::to_string(m_iFps);
}

double CTimer::getElapsedSeconds()
{
	static Uint32 starttime = 0;
	Uint32 currenttime;

	/* Get the current ticks */
	currenttime = SDL_GetTicks();

	/* Get the seconds pass between calls to getElapsedSeconds */
	double secs = ((double)currenttime - (double)starttime) / 1000.0;

	/* Reset the timer */
	starttime = currenttime;

	/* Get Frames Per Second */
	m_iFps = (int)(1.0 / secs);

	return secs;
}

bool CTimer::isStarted() 
{ 
	//Timer is running and paused or unpaused 
	return m_bStarted; 
} 

bool CTimer::isPaused() 
{ 
	//Timer is running and paused 
	return m_bPaused && m_bStarted; 
}