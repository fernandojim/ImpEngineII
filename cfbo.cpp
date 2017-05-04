/*
 * cfbo.cpp
 *
 *  Created on: 26 de abr. de 2017
 *      Author: fjimartinez
 */
#include "stdafx.h"


#include "cfbo.h"

CFbo::CFbo()
{
	// TODO Auto-generated constructor stub
	clean();

	//Creates the buffer
	Create();
}

CFbo::CFbo(GLuint w, GLuint h)
{
	clean();

	m_width = w;
	m_height = h;

	//Creates the buffer
	Create();
}

CFbo::~CFbo()
{
	// TODO Auto-generated destructor stub
	clean();
}

void CFbo::Create()
{
	//Generate and bind the frame buffer
	glGenFramebuffers(1, &m_uiFboHandler);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFboHandler);

	//Generate and bind the texture
	glGenTextures(1, &m_uiTextureHandler);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureHandler);
#ifdef _OPENGL44_
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB, m_width, m_height);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Bind the texture to Frame Buffer Object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTextureHandler, 0);

	//Create the stencil buffer
	GLuint rboDepthStencil;
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

	//Create the depth buffer
	glGenRenderbuffers(1, &m_uiDepthHandler);
	glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthHandler);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthHandler);

	//Set the target for the fragment shader output
	GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBufs);

	// Always check that our framebuffer is ok
	m_buffererror = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	//Unbind the buffers and revert to default
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFbo::BindFBO()
{
	//Bind the Frame Buffer Object to its usage
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFboHandler);

	// adjust viewport and projection matrices to texture dimensions
	glViewport(0, 0, m_width, m_height);

	//Clean the color and depth buffers
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CFbo::UnbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFbo::clean()
{
	m_uiFboHandler = 0;
	m_uiDepthHandler = 0;
	m_uiTextureHandler = 0;
    m_width = 0;
    m_height = 0;
}

