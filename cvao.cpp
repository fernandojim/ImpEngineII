#include "stdafx.h"

#include "cvao.h"

#define BUFFER_OFFSET(i) ((unsigned int*) NULL + (i))

CVao::CVao()
{
	m_handler = 0;

	/* Creates an arrays object */
	glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);
}

void CVao::CreateArrayBuffer(void *buffer, GLuint size, GLenum usage)
{
	/* Generates an id for buffer and get its id into 'vbo' */
	glGenBuffers(1, &m_vboHandles[m_handler]);

	/* Depending on nElements creates the buffer */
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandles[m_handler]);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, usage);
}

void CVao::CreateAttribArrayBuffer(GLuint attrib_id, GLuint elements)
{
	/* Asociates an attrib id to a array buffer */
	glEnableVertexAttribArray(attrib_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandles[m_handler]);
	glVertexAttribPointer(attrib_id, elements, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	//Prepate to next array buffer
	m_handler++;
}

CVao::~CVao()
{
	
}