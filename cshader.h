#ifndef _CSHADER_H_
#define _CSHADER_H_

#include <iostream>
#include "gl_core_3_3.h"

#define MAX_INFO_LOG 255
#define MAX_UNIFORMS 10

#define BUFFER_OFFSET(i) ((unsigned int*) NULL + (i))

enum ERROR_TYPE { NO_ERR = 0, 
				  FILE_ERROR, 
				  VS_COMPILE_ERROR,
				  FS_COMPILE_ERROR,
				  PROGRAM_LINK_ERROR,
};

class CShader 
{
	public:
		CShader(const char* vs, const char* fs) { m_err = CreaShaders(vs, fs); }
		~CShader() {}

		ERROR_TYPE getError();

		/********************************/
		/*  Atributos para los shaders  */
		/********************************/
		GLuint	m_uiProgram;

		// Uniforms location array
		GLint   m_iUniformLocation[MAX_UNIFORMS];

		ERROR_TYPE CreaShaders(const char* vs, const char* fs);

	private:
		/**************************************/
		/* Para compilar y linkar los shaders */
		/**************************************/
		ERROR_TYPE m_err;

		const char *m_sVertSource;
		const char *m_sFragSource;

		GLuint m_uiVertexShader;
		GLuint m_uiFragmentShader;

		GLint  m_iCompileStatusVertexShader;
		GLint  m_iCompileStatusFragmentShader;
		GLchar m_sLogCompileStatusVertexShader[MAX_INFO_LOG];
		GLchar m_sLogCompileStatusFragmentShader[MAX_INFO_LOG];
		
		GLint  m_iLinkStatusShaders;
		GLchar m_sLogLinkStatusShaders[MAX_INFO_LOG];
		
		ERROR_TYPE CompilaShaders();
		ERROR_TYPE LinkaShaders();

		GLchar* GetBufferFromFile( const char* in_szFilename );
};

#endif
