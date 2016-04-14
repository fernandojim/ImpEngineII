/*
	NAMESPACE TO DEFINE VERTEX, NORMALS AND TEXELS 
*/
#ifndef _NVERTEX_H_
#define _NVERTEX_H_

#include "gl_core_3_3.h"

namespace NVERTEX
{
	struct Vertex3
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
	};

	struct Vertex2
	{
		GLfloat x;
		GLfloat y;
	};
}

#endif