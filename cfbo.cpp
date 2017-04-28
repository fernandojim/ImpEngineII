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

void CFbo::Create2()
{
	glGenRenderbuffers(1, &m_uiColorHandler);
	glBindRenderbuffer(GL_RENDERBUFFER, m_uiColorHandler );
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, m_width, m_height );

	glGenRenderbuffers(1, &m_uiDepthHandler);
	glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthHandler);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

	glGenFramebuffers(1, &m_uiFboHandler);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFboHandler);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_uiColorHandler);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthHandler);

	//Generate and bind the texture
	glGenTextures(1, &m_uiTextureHandler);
	glActiveTexture(GL_TEXTURE0 + m_uiTextureHandler);
	glBindTexture(GL_TEXTURE, m_uiTextureHandler);

#ifdef _OPENGL44_
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, m_width, m_height);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Bind the texture to Frame Buffer Object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTextureHandler, 0);

	GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBufs);

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void CFbo::Create()
{
	//Generate and bind the frame buffer
	glGenFramebuffers(1, &m_uiFboHandler);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFboHandler);

	//Generate and bind the texture
	glGenTextures(1, &m_uiTextureHandler);
	glActiveTexture(GL_TEXTURE0 + m_uiTextureHandler);
	glBindTexture(GL_TEXTURE, m_uiTextureHandler);
#ifdef _OPENGL44_
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB, m_width, m_height);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glGenRenderbuffers(1, &m_uiColorHandler);

	//Bind the texture to Frame Buffer Object
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTextureHandler, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_uiTextureHandler, 0);

	//Create the depth buffer
	glGenRenderbuffers(1, &m_uiDepthHandler);
	glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthHandler);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthHandler);

	//Set the target for the fragment shader output
	GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBufs);

	//Unbind the buffers and revert to default
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Always check that our framebuffer is ok
	m_buffererror = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	//Create one pixel white texture
	/*GLubyte whiteTex[] = { 255, 255, 255, 255 };
	glGenTextures(1, &m_uiWhiteTexture);
	glActiveTexture(GL_TEXTURE0 + m_uiWhiteTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiWhiteTexture);
#ifdef _OPENGL44_
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, 1, 1);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
#endif
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, whiteTex);*/
}

void CFbo::BindFBO()
{
	//Bind the Frame Buffer Object to its usage
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFboHandler);
	glViewport(0, 0, m_width, m_height);
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
    m_uiWhiteTexture = 0;
    m_width = 0;
    m_height = 0;
}

