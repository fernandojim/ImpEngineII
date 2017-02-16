#ifndef _CSHADER_H_
#define _CSHADER_H_

#include <iostream>
#include <fstream>
#include <string>
#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif
#include "utils.h"

using std::string;
using std::ifstream;
using namespace Message;

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
		CShader(const char* vs, const char* fs);
		~CShader() {}

		ERROR_TYPE getError();

		/*****************************/
		/* Shaders public attributes */
		/*****************************/

		// Program identifier -for use in drawing meshes-
		GLuint	m_uiProgram;

		// Uniforms location array
		GLint   m_iUniformLocation[MAX_UNIFORMS];

		// Uniforms variables
		void    setUniformShader1f(const string variable, GLfloat v1);
		void    setUniformShader1i(const string variable, GLint v1);
		void    setUniformShader2fv(const string variable, GLsizei count, const GLfloat *value);
		void    setUniformShader3fv(const string variable, GLsizei count, const GLfloat *value);
		void    setUniformShaderMatrix3fv(const string &variable, GLsizei count, GLboolean traspose, const GLfloat *value);
		void    setUniformShaderMatrix4fv(const string variable, GLsizei count, GLboolean traspose, const GLfloat *value);
		void 	setUniformShaderTexture2D(const string variable, GLuint textureid);

		ERROR_TYPE CreaShaders(const char* vs, const char* fs);
	private:
		/***********************************/
		/* To compile and link the shaders */
		/***********************************/
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
