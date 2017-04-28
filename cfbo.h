/*
 * cfbo.h
 *
 *  Created on: 26 de abr. de 2017
 *      Author: fjimartinez
 */

/*
 * Create an Frame Buffer Object (FBO)
 */

#ifndef _CFBO_H_
#define _CFBO_H_

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

class CFbo
{
public:
	/* Handler to Frame Bufffer */
	GLuint m_uiFboHandler;

	/* Handler to buffers */
	GLuint m_uiColorHandler;
	GLuint m_uiDepthHandler;

	/* Handler to Texture */
	GLuint m_uiTextureHandler;

	/* Handler to white Texture */
	GLuint m_uiWhiteTexture;

	/* Size */
	GLuint m_width;
	GLuint m_height;

	GLuint m_buffererror;

public:
	CFbo();
	CFbo(GLuint w, GLuint h);
	virtual ~CFbo();

	void Create();
	void Create2();
	void BindFBO();
	void UnbindFBO();

	void clean();
};

#endif /* CFBO_H_ */
