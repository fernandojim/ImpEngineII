#ifndef _CFONT_H_
#define _CFONT_H_

#include "cshader.h"
#include "cvao.h"
#include "ctexture.h"
#include "glm\vec2.hpp"
#include "nvertex.h"

#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //Header for glm::perspective, etc...
#include "glm/gtc/type_ptr.hpp"

#include "texturemanager.h"

#define TEXT_VS      "shaders\\text.vertex"			//Vertex shader file
#define TEXT_FS      "shaders\\text.fragment"       //Fragment shader file

#define N_VERTEX	     4
#define ANCHO_CELDA  	 16							//Ancho de cada celda en pixels
#define DESPLAZAMIENTO_X 1									 
#define DESPLAZAMIENTO_Y 1

using namespace NVERTEX;

class CFont
{
private:
	string    m_text;

	GLuint    m_uiTextura; // id de la textura
		
	//Vertex/Texels/Index info
	Vertex2* m_Vertex;
	Vertex2* m_Texel;
	GLuint*  m_Index;

	//Matrix
	glm::mat4 m_projection;
	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_mvp;

	//Shader para el renderizado
	CShader   *m_pShader;

	//Objeto Vertex Array Buffer - GPU vertex
	CVao      *m_pVAO;

private:
	DWORD m_count;
		
public:
	CFont(const char *vs, const char *fs);
	~CFont();

	void Render(string text);

	void setIdTexture(GLuint id);
};

#endif
