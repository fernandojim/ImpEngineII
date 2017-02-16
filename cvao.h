#ifndef _CVAO_H_
#define _CVAO_H_

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

#define MAX_BUFFERS 6

class CVao
{
public:
	GLuint m_vaoHandle;
	GLuint m_vboHandles[MAX_BUFFERS];

	GLuint m_handler;

public:
	CVao();
	~CVao();

	// Method for create Arrays of Buffer Objects
	void CreateArrayBuffer(void *buffer, GLuint size, GLenum usage);

	// Method for create an attrib to an array buffer to Shader
	void CreateAttribArrayBuffer(GLuint attrib_id, GLuint elements);
};

#endif
