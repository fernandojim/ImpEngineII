#include "stdafx.h"

#include "cshader.h"
#include "utils.h"

using namespace Message;

CShader::CShader(const char* vs, const char* fs)
{
	m_err = CreaShaders(vs, fs);
}

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

	//Obtiene y visualiza el mensaje de error
	if ( !m_iCompileStatusVertexShader )
	{
		glGetShaderInfoLog(m_uiVertexShader, MAX_INFO_LOG, NULL, m_sLogCompileStatusVertexShader);

		MessageBOX("Vertex Shader compile error", m_sLogCompileStatusVertexShader);

		return VS_COMPILE_ERROR;
	}
	
	//Compila el Fragment Shader
	m_uiFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_uiFragmentShader, 1, &m_sFragSource, NULL);
    glCompileShader(m_uiFragmentShader);
    glGetShaderiv(m_uiFragmentShader, GL_COMPILE_STATUS, &m_iCompileStatusFragmentShader);

    //Visualiza mensaje de error
	if ( !m_iCompileStatusFragmentShader )
	{
		glGetShaderInfoLog(m_uiFragmentShader, MAX_INFO_LOG, NULL, m_sLogCompileStatusFragmentShader);

		MessageBOX("Fragment Shader compile error", m_sLogCompileStatusFragmentShader);

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
			MessageBOX("Shader linker error", m_sLogLinkStatusShaders);
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

GLchar* CShader::GetBufferFromFile( const char* in_szFilename )
{
	ifstream file;
	int size;
	char *buffer;

	//It is important to set the binary mode
	file.open(in_szFilename, std::ios::binary);

	if (file.is_open())
	{
		//Set at the end of the file to calculate the size
		file.seekg(0, std::ios::end);

		//Calculate its size in bytes
		size = file.tellg();

		//Get back to the begining
		file.seekg(0, std::ios::beg);

		//Create the buffer
		buffer = new char[size + 1];

		//Read it
		file.read((char*)&buffer[0], size);
		buffer[size] = '\0';

		//Closes the file
		file.close();

		return buffer;
	}

	return 0;
}

ERROR_TYPE CShader::getError()
{
	return m_err;
}

void CShader::setUniformShader1f(const string variable, GLfloat v1)
{
	GLint val = glGetUniformLocation(m_uiProgram, variable.c_str());
	glUniform1f(val, v1);
}

void CShader::setUniformShader1i(const string variable, GLint v1)
{
	GLint val = glGetUniformLocation(m_uiProgram, variable.c_str());
	glUniform1i(val, v1);
}

void CShader::setUniformShader2fv(const string variable, GLsizei count, const GLfloat *value)
{
	GLint val = glGetUniformLocation(m_uiProgram, variable.c_str());
	if (val == -1)
	glUniform2fv(val, count, &value[0]);
}

void CShader::setUniformShader3fv(const string variable, GLsizei count, const GLfloat *value)
{
	GLint val = glGetUniformLocation(m_uiProgram, variable.c_str());
	glUniform3fv(val, count, &value[0]);
}

void CShader::setUniformShaderMatrix3fv(const string &variable, GLsizei count, GLboolean traspose, const GLfloat *value)
{
	GLint val = glGetUniformLocation(m_uiProgram, variable.c_str());
	glUniformMatrix3fv(val, count, traspose, &value[0]);
}

void CShader::setUniformShaderMatrix4fv(const string variable, GLsizei count, GLboolean traspose, const GLfloat *value)
{
	GLint val = glGetUniformLocation(m_uiProgram, variable.c_str());
	glUniformMatrix4fv(val, count, traspose, &value[0]);
}

void CShader::setUniformShaderTexture2D(const string variable, GLuint textureid)
{
	GLint val = glGetUniformLocation(m_uiProgram, variable.c_str());
	glUniform1i(val, textureid);
	glActiveTexture(GL_TEXTURE0 + textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);
}


