#include "stdafx.h"

#include "cshader.h"
#include "utils.h"

using namespace ansiCToUnicode;

ERROR_TYPE CShader::CreaShaders(const char* vs, const char* fs)
{
	ERROR_TYPE err = NO_ERR;

	m_sVertSource = GetBufferFromFile(vs);
	m_sFragSource = GetBufferFromFile(fs);

	if (m_sVertSource == 0 || m_sFragSource == 0)
	{
		err = FILE_ERROR;
	}
	else
	{
		err = CompilaShaders();

		if (err == NO_ERR)
			err = LinkaShaders();
	}

	return err;
}

ERROR_TYPE CShader::CompilaShaders()
{
	//Compila el Vertex Shader
	m_uiVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_uiVertexShader, 1, &m_sVertSource, NULL);
    glCompileShader(m_uiVertexShader);
    glGetShaderiv(m_uiVertexShader, GL_COMPILE_STATUS, &m_iCompileStatusVertexShader);

	//Obtiene el mensaje de error
	if ( !m_iCompileStatusVertexShader )
	{
		glGetShaderInfoLog(m_uiVertexShader, MAX_INFO_LOG, NULL, m_sLogCompileStatusVertexShader);

		//Visual C++ 2013
		//Visualiza mensaje de error
		//MessageBox(NULL, ansiCToUnicode::ansiCToUnicode(m_sLogCompileStatusVertexShader).c_str(),
		//				 ansiCToUnicode::ansiCToUnicode("Vertex Shader compile error").c_str(), MB_OK);
		
		//Dev-C++ 5.11				 
		MessageBox(NULL, m_sLogCompileStatusVertexShader, "Vertex Shader compile error", MB_OK);

		return VS_COMPILE_ERROR;
	}
	
	//Compila el Fragment Shader
	m_uiFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_uiFragmentShader, 1, &m_sFragSource, NULL);
    glCompileShader(m_uiFragmentShader);
    glGetShaderiv(m_uiFragmentShader, GL_COMPILE_STATUS, &m_iCompileStatusFragmentShader);
	if ( !m_iCompileStatusFragmentShader )
	{
		glGetShaderInfoLog(m_uiFragmentShader, MAX_INFO_LOG, NULL, m_sLogCompileStatusFragmentShader);

		//Visual C++ 2013
		//Visualiza mensaje de error
		//MessageBox(NULL, ansiCToUnicode::ansiCToUnicode(m_sLogCompileStatusFragmentShader).c_str(),
		//				 ansiCToUnicode::ansiCToUnicode("Fragment Shader compile error").c_str(), MB_OK);
						 
		//Dev-C++ 5.11
		MessageBox(NULL, m_sLogCompileStatusVertexShader, "Fragment Shader compile error", MB_OK);

		return FS_COMPILE_ERROR;
	}

	return NO_ERR;
}
	
ERROR_TYPE CShader::LinkaShaders()
{
	m_uiProgram = glCreateProgram();
    glAttachShader(m_uiProgram, m_uiVertexShader);
    glAttachShader(m_uiProgram, m_uiFragmentShader);
	glLinkProgram(m_uiProgram);
    glGetProgramiv(m_uiProgram, GL_LINK_STATUS, &m_iLinkStatusShaders);

	//Comprueba si se ha linkado correctamente
	if (m_iLinkStatusShaders == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_uiProgram, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		glGetProgramInfoLog(m_uiProgram, maxLength, &maxLength, m_sLogLinkStatusShaders);

		//Visualiza mensaje de error
		if (strlen(m_sLogLinkStatusShaders) > 0)
		{
			//Visual C++ 2013
			//MessageBox(NULL, ansiCToUnicode::ansiCToUnicode(m_sLogLinkStatusShaders).c_str(),
			//				 ansiCToUnicode::ansiCToUnicode("Shader linker error").c_str(), MB_OK);
			
			//Dev-C++ 5.11
			MessageBox(NULL, m_sLogLinkStatusShaders, "Shader linker error", MB_OK);
		}
		
		//We don't need the program anymore.
		glDeleteProgram(m_uiProgram);

		//Don't leak shaders either.
		glDeleteShader(m_uiVertexShader);
		glDeleteShader(m_uiFragmentShader);

		//Use the infoLog as you see fit.

		//In this simple program, we'll just leave
		return PROGRAM_LINK_ERROR;
	}

	return NO_ERR;
}

char* CShader::GetBufferFromFile( const char* in_szFilename )
{
	FILE* pFile = NULL;

	if( (pFile = fopen_s( in_szFilename, "rb" )) != NULL )
	{
		fseek( pFile, 0, SEEK_END );
		unsigned int nSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		char* pBuffer = new char[nSize+1];
		fread( pBuffer, sizeof(char), nSize, pFile );
		fclose( pFile );

		pBuffer[nSize] = '\0';

		return pBuffer;                         
	}

	return 0;
}

ERROR_TYPE CShader::getError()
{
	return m_err;
}
