// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <SDL.h>

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

#define GLM_FORCE_RADIANS

// TODO: reference additional headers your program requires here
