#include "stdafx.h"

#include "cfont.h"

using namespace TextureManager;

CFont::CFont(const char *vs, const char *fs)
{
	//Inicializa los arrays
	m_Vertex = new Vertex2[N_VERTEX];
	m_Texel = new Vertex2[N_VERTEX];
	m_Index = new GLuint[N_VERTEX];

	m_count = 0;
	m_uiTextura = 0;

	/* Crea lista de vértice del quadrado */
	m_Vertex[0].x = 0.0;
	m_Vertex[0].y = 114.0;
	m_Vertex[1].x = 16.0;
	m_Vertex[1].y = 114.0;
	m_Vertex[2].x = 0.0;
	m_Vertex[2].y = 128.0;
	m_Vertex[3].x = 16.0;
	m_Vertex[3].y = 128.0;
		
	/* Crea lista de coordenadas de textura */
	m_Index[0] = 0;
	m_Index[1] = 1;
	m_Index[2] = 2;
	m_Index[3] = 3;

	m_text = " ";
	
	/* Crea las matrices */
	//La proyección será ortográfica por tratarse de un objeto fijo e inmovil.
	m_view = glm::lookAt(glm::vec3(0.0, 0.0, 90.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 100.0f, -100.0f);

	//Crea los shader asociados a este objeto
	m_pShader = new CShader("shaders\\fonts.vertex", "shaders\\fonts.fragment");

	//Crea el objeto VAO
	m_pVAO = new CVao();

	//Crea los buffers del objeto VAO
	m_pVAO->CreateArrayBuffer(m_Vertex, N_VERTEX * sizeof(Vertex2), GL_STATIC_DRAW);
	m_pVAO->CreateAttribArrayBuffer(3, 2);

	m_pVAO->CreateArrayBuffer(m_Texel, N_VERTEX * sizeof(Vertex2), GL_STREAM_DRAW);
	m_pVAO->CreateAttribArrayBuffer(4, 2);

	//Carga la textura
	m_uiTextura = ::getTextureManager().loadTexture("texturas\\fonts_A_Z_0_9.bmp");
}

CFont::~CFont()
{
	delete m_pShader;
	delete m_pVAO;
}

void CFont::Render(string text)
{
	GLuint chr;
	GLuint base;
	GLushort ix, iy;
	static DWORD initCount = 0;
	int despx = 0;
	int despy = 0;
	
	m_count = GetTickCount();

	//Cada 125ms actualizamos la informacion a visualizar
	if (m_count - initCount > 125)
	{
		m_text = text;
		initCount = m_count;
	}

	glUseProgram(m_pShader->m_uiProgram);

	//Renderiza si el tamaño del texto es mayor que 0
	for (GLuint i = 0; i < m_text.length(); i++)
	{
		//Obtenemos el código del carácter a dibujar
		chr = m_text.at(i);

		if (m_text.at(i) == '<')
		{
			despy += 16;
			despx = 0;
			chr = m_text.at(++i);
		}
		
		ix = 0;
		iy = 0;

		/* Del código del carácter se obtiene el índice para el desplazamiento en la textura */
		if (chr >= 48 && chr <= 53)		// 0..5
		{
			ix = chr - 22;
		}
		else if (chr >= 54 && chr <= 57)   // 6..9
		{
			ix = chr - 22 - 32;
			iy = 1;
		}
		else if ((chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122))	// A..Z o a..z
		{
			if (chr >= 97)
				ix = chr - 97; //Minúsculas
			else
				ix = chr - 65; //Mayúsculas
		}
		else if (chr == 45)	   //guión
		{
			ix = 1;
			iy = 2;
		}
		else if (chr == 46)				// punto
		{
			ix = 0;
			iy = 2;
		}
		else if (chr == 32)				    // Espacio en blanco
		{
			ix = 32;
			iy = 7;
		}

		//Mapeamos el buffer de texels para acceder a la textura que representa la letra elegida
		glBindBuffer(GL_ARRAY_BUFFER, m_pVAO->m_vboHandles[1]);
		m_Texel = (Vertex2*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		//Para cada caracter, elegimos las coordenadas de textura de su letra
		m_Texel[0].x = ix / 32.0f;
		m_Texel[0].y = (8.0f - iy - 1.0f) / 8.0f;
		m_Texel[1].x = (ix + 1) / 32.0f;
		m_Texel[1].y = (8.0f - iy - 1.0f) / 8.0f;
		m_Texel[2].x = ix / 32.0f;
		m_Texel[2].y = (8.0f - iy) / 8.0f;
		m_Texel[3].x = (ix + 1.0f) / 32.0f;
		m_Texel[3].y = (8.0f - iy) / 8.0f;

		//Finaliza la modificación del buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);

		//Crea la matrix Model-View-Projection
		m_model = glm::translate(glm::mat4(1.0), glm::vec3(0.0 + (ANCHO_CELDA * despx) + DESPLAZAMIENTO_X, 0.0 - despy, 0.0));
		m_mvp = m_projection * m_view * m_model;

		//Get the texture id
		GLuint val = glGetUniformLocation(m_pShader->m_uiProgram, "texturaFonts");
		glUniform1i(val, m_uiTextura);
		glActiveTexture(GL_TEXTURE0 + m_uiTextura);
		glBindTexture(GL_TEXTURE_2D, m_uiTextura);

		//Matrix de transformación
		base = glGetUniformLocation(m_pShader->m_uiProgram, "mvp");
		glUniformMatrix4fv(base, 1, GL_FALSE, glm::value_ptr(m_mvp));

		glBindVertexArray(m_pVAO->m_vaoHandle);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		despx++;
	} //End-for (GLuint i = 0; i < m_iNumFonts; i++)
}

void CFont::setIdTexture(GLuint id)
{
	m_uiTextura = id;
}
