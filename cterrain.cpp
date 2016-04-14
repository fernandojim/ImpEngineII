#include "stdafx.h"

#include "cterrain.h"

#define PUNTOS_ALREDEDOR 6

//------------------------------------------
// Constructor by file
//------------------------------------------
CTerrain::CTerrain(const char *file) : CObject()
{
	GLuint x, z, k;
	float escala = 0.0;
	float rgb_max = 0.0;

	/* Create the File Object */
	m_File = new CFileDef(file);
	m_File->getObjectKeysValues();

	/* Read the content of file */
	if (m_File)
	{
		/* Gets the name ob object */
		m_sName = m_File->getObjectName();

		/* Gets the properties */
		m_vs = m_File->getObjectValues("vs")[0];
		m_fs = m_File->getObjectValues("fs")[0];
		m_fAncho = std::stof(m_File->getObjectValues("ancho")[0]);
		m_fLargo = std::stof(m_File->getObjectValues("largo")[0]);
		m_fAlturaMax = std::stof(m_File->getObjectValues("altura")[0]);
		m_fTilingFactor = std::stof(m_File->getObjectValues("tilingfactor")[0]);
		m_texturaHeightfield = m_File->getObjectValues("heightfield")[0];
	
		//Carga la imagen del mapa de alturas
		m_pHeightMap = new CBmp();
		m_pHeightMap->loadBMP(m_texturaHeightfield.c_str());

		//Si la textura es válida, toma el ancho de esta como m_iDivisiones
		if (m_pHeightMap != NULL)
			m_iDivisiones = m_pHeightMap->m_lHeight;

		//Calcula el número total de vértices y crea la lista
		m_iNumVertex = m_iDivisiones * m_iDivisiones;
		m_Vertex = new Vertex3[m_iNumVertex];

		//Una normal por cada vértice
		m_Normal = new Vertex3[m_iNumVertex];

		//Calcula el número total de indices de vértices y crea la lista
		m_iNumIndex = (m_iDivisiones * 2) * (m_iDivisiones - 1);
		m_Index = new GLuint[m_iNumIndex];

		//Crea la lista de texels (coordenadas de textura)
		m_Texel = new Vertex2[m_iNumVertex];

		//Calcula el valor rgb máximo 0-255
		k = 0;
		rgb_max = 0.0;
		for (x = 0; x < m_iDivisiones; x++)
		{
			for (z = 0; z < m_iDivisiones; z++)
			{
				if (m_pHeightMap->m_pPixelData[k * 3] > rgb_max)
					rgb_max = m_pHeightMap->m_pPixelData[k * 3];

				k++;
			}
		}

		//escala la altura máxima del terreno a la altura introducida por el parámetro 'altura'
		escala = m_fAlturaMax / rgb_max;

		//Se crea el terreno como una rejilla de lado divisiones con forma plana
		k = 0;
		for (x = 0; x < m_iDivisiones; x++)
		{
			for (z = 0; z < m_iDivisiones; z++)
			{
				m_Vertex[k].x = x * m_fAncho;
				m_Vertex[k].y = m_pHeightMap->m_pPixelData[k * 3] * escala;
				m_Vertex[k].z = z * m_fLargo;

				m_Texel[k].x = static_cast<float>(x);// / static_cast<float>(m_iDivisiones);
				m_Texel[k].y = static_cast<float>(z);// / static_cast<float>(m_iDivisiones);

				k++;
			}
		}

		/*
		Se crea ahora la lista de índices para poder referenciar a cada vértice
		3 ---<--- 2 
		        /
		      /
		    ^
		  /
		1 ---<--- 0
		*/
		k = 0;
		for (x = 0; x < m_iDivisiones - 1; x++)
		{
			for (z = 0; z < m_iDivisiones; z++)
			{
				m_Index[k++] = (m_iDivisiones * (x + 1)) + z;
				m_Index[k++] = (m_iDivisiones *  x) + z;
			}
		}

		//Después de crear los vértices del terreno, calculo las normales
		for (GLuint cont = 0; cont < m_iNumVertex; cont++)
		{
			m_Normal[cont] = CalculaNormalVertice(cont);
		}

		//Crea el shader asociado al terreno
		m_pShader = new CShader(m_vs.c_str(), m_fs.c_str());

		//Comprueba si se creó correctamente
		if (m_pShader->getError() != ERROR_TYPE::NO_ERR)
			exit(0);

		//Crea el objeto VAO
		m_pVao = new CVao();

		//Crea los buffers del objeto VAO
		m_pVao->CreateArrayBuffer(m_Vertex, m_iNumVertex * sizeof(Vertex3), GL_STATIC_DRAW);
		m_pVao->CreateAttribArrayBuffer(0, 3);

		m_pVao->CreateArrayBuffer(m_Normal, m_iNumVertex * sizeof(Vertex3), GL_STATIC_DRAW);
		m_pVao->CreateAttribArrayBuffer(1, 3);

		m_pVao->CreateArrayBuffer(m_Texel, m_iNumVertex * sizeof(Vertex2), GL_STATIC_DRAW);
		m_pVao->CreateAttribArrayBuffer(2, 2);

		//El terreno es el objeto padre
		m_pParent = NULL;
	}//Object file not valid or it has no content
}

CTerrain::~CTerrain()
{
	delete m_pShader;
	delete m_pHeightMap;
}

void CTerrain::Render()
{
	GLint val;
	GLint offset;
	GLint size;

	glUseProgram(m_pShader->m_uiProgram);
	{
		//Crear variables a pasar al shader
		//
		//Carga las texturas para el shader
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "texturasand");
		glUniform1i(val, 1);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, 1);

		val = glGetUniformLocation(m_pShader->m_uiProgram, "texturagrass");
		glUniform1i(val, 2);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, 2);
		
		val = glGetUniformLocation(m_pShader->m_uiProgram, "texturadirt");
		glUniform1i(val, 3);
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, 3);

		val = glGetUniformLocation(m_pShader->m_uiProgram, "texturasnow");
		glUniform1i(val, 4);
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, 4);

		val = glGetUniformLocation(m_pShader->m_uiProgram, "texturanoise");
		glUniform1i(val, 5);
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, 5);

		//
		//Luces
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.lightPosition");
		glUniform3fv(val, 1, glm::value_ptr(m_world->m_Lights[0].m_light.lightPosition));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.lightIntensity");
		glUniform3fv(val, 1, glm::value_ptr(m_world->m_Lights[0].m_light.lightIntensity));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.ka");
		glUniform3fv(val, 1, glm::value_ptr(m_world->m_Lights[0].m_light.ka));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.kd");
		glUniform3fv(val, 1, glm::value_ptr(m_world->m_Lights[0].m_light.kd));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.ks");
		glUniform3fv(val, 1, glm::value_ptr(m_world->m_Lights[0].m_light.ks));
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "luz1.shininess");
		glUniform1f(val, m_world->m_Lights[0].m_light.shininess);

		//
		//Otros
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "tilingfactor");
		glUniform1f(val, m_fTilingFactor);

		val = glGetUniformLocation(m_pShader->m_uiProgram, "altmax");
		glUniform1f(val, m_fAlturaMax);

		//
		//Matrices
		//
		val = glGetUniformLocation(m_pShader->m_uiProgram, "m3normal");
		glUniformMatrix3fv(val, 1, GL_FALSE, glm::value_ptr(m_camera->m_M3normal));

		val = glGetUniformLocation(m_pShader->m_uiProgram, "m4modelview");
		glUniformMatrix4fv(val, 1, GL_FALSE, glm::value_ptr(m_camera->m_M4modelView));

		val = glGetUniformLocation(m_pShader->m_uiProgram, "m4mvp");
		glUniformMatrix4fv(val, 1, GL_FALSE, glm::value_ptr(*m_WorldTransformation));

		glBindVertexArray(m_pVao->m_vaoHandle);
		size = m_iDivisiones * 2;
		for (GLuint i = 0; i < m_iDivisiones - 1; i++)
		{
			offset = size * i;
			glDrawElements(GL_TRIANGLE_STRIP, size, GL_UNSIGNED_INT, &m_Index[offset]);
		}
		glBindVertexArray(0);
	}
	glUseProgram(0);
}

float CTerrain::GetAltura(float x, float z)
{
    glm::vec3 v1, v2, v3;
	glm::vec3 pos = glm::vec3(x, 0.0, z);
    float A,B,C,D;
    
    //Obtenemos los índices de los puntos del triángulo en el que está la pos x,z
    obtieneIndicesTriangulo(pos, v1, v2, v3);
    
    //Calculamos A, B, C, D de la ecuación Ax + By + Cz = D del plano
    //    A,B,C representa las magnitudes de la normal del triángulo calculado
    A = v1.y * (v2.z - v3.z) + v2.y * (v3.z - v1.z) + v3.y * (v1.z - v2.z);
    B = v1.z * (v2.x - v3.x) + v2.z * (v3.x - v1.x) + v3.z * (v1.x - v2.x);
    C = v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y);
    D = -(v1.x * (v2.y * v3.z - v3.y * v2.z) + v2.x * (v3.y * v1.z - v1.y * v3.z) + v3.x * (v1.y * v2.z - v2.y * v1.z));
    
    //Y ahora la coordenada Y de nuestro punto x,y dentro de ese plano calculado
    if (B == 0.0f) 
       B = 0.1f;
       
    return -(D + A * x + C * z) / B;
}

GLuint CTerrain::getIndice(GLuint x, GLuint z)
{
     //(x, z) indica la coordenada de vértice de la rejilla de vértices
     return (z + (x * m_iDivisiones));
}
   
void CTerrain::setAltura(GLuint indice, float altura)
{
     float alt;
     
	 if (indice < 0 || indice > m_iNumVertex) //Sale del rango
        return;
     else
     {
         alt = altura * 4 / 5;
		 m_Vertex[indice].y += alt;
         
         if (puntoInterior(indice))
         {
			 m_Vertex[indice - 1].y += altura * 1 / 5;
             
             //Nos aseguramos que no sobrepasar el número de vértices
			 if (indice < m_iNumVertex - 1)
				 m_Vertex[indice + 1].y += altura * 1 / 5;
             
			 m_Vertex[indice - m_iDivisiones].y += altura * 1 / 5;
             
             //Nos aseguramos que no sobrepasar el número de vértices
			 if (indice < m_iNumVertex - m_iDivisiones - 1)
				 m_Vertex[indice + m_iDivisiones].y += altura * 1 / 5;
         }
     }
}

bool CTerrain::puntoInterior(GLuint ind)
{
	GLuint x, z; //Coordenadas x, z calculadas a partir del indice 'ind'
     
	if (ind < 0 || ind > m_iNumVertex)
		return false;
    else
    {
        //Calcula las coordenadas a partir del índice
        z = ind % m_iDivisiones;
        x = ind / m_iDivisiones;
         
        //Comprobamos que la coordenada x,z corresponde a un punto interior
        if (x > 0 && x < m_iDivisiones && z > 0 && z < m_iDivisiones)
           return true;
    }
    
	return false;
}

glm::vec3 CTerrain::calculaNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 vector1;
	glm::vec3 vector2;
	glm::vec3 vector3;

	/* Creamos los vectores según los puntos introducidos */
	vector1 = p2 - p1;
    vector2 = p3 - p1;

	/* El producto vectorial entre dos vectores es la normal al plano que forman */
	return glm::normalize(glm::cross(vector2, vector3));
}

void CTerrain::obtieneIndicesTriangulo(glm::vec3 pos, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3)
{
     //Obtenemos las coordenadas de vértices según la posición absoluta
	GLuint xdiv = static_cast<GLuint>(fabs(pos.x / m_fAncho));
	GLuint zdiv = static_cast<GLuint>(fabs(pos.z / m_fLargo));
     
    //Comprobamos que estamos dentro del terreno
    if (xdiv < 0 || xdiv > m_iDivisiones - 1 || zdiv < 0 || zdiv > m_iDivisiones - 1)
       return;
     
    //Calculamos la posicion relativa a la celda en la que estamos
    float pos_rel_x = pos.x - ( (float)xdiv * m_fAncho );
    float pos_rel_z = pos.z - ( (float)zdiv * m_fLargo );
     
    //Obtenemos el triangulo en el que está la posición
	GLuint triangulo = 0;
	if ( pos_rel_z > pos_rel_x )
       triangulo = 1;
        
	//triangulo = 1 implica que del siguiente modelo de celda del terreno
	//   3 --- 2
	//   |   / |
	//   | /   |
	//   1 --- 0
	//   tomaremos el triángulo formado por los vértices 0-1-3.
	//   El triángulo 0 esta formado entonces por 2-0-3.
	//   ya que nuestro punto x,z cae dentro de dicho triángulo
	//   por haberlo sacado de los cálculos anteriores
	//   ind1, ind1 e ind3 se refiere a los índices en la lista de vértices del terreno 
	//Calculamos los índices de los vértices del triángulo en el que estamos
	GLuint p1, p2, p3;
    if (triangulo == 1) 
    {
		p1 = ( m_iDivisiones * xdiv ) + zdiv;
        p2 = ( m_iDivisiones * ( xdiv + 1 ) ) + zdiv + 1;
        p3 = ( m_iDivisiones * xdiv ) + zdiv + 1;
    }
    else
    {
        p1 = ( m_iDivisiones * ( xdiv + 1 ) ) + zdiv;
        p2 = ( m_iDivisiones * xdiv ) + zdiv;
        p3 = ( m_iDivisiones * ( xdiv + 1 ) ) + zdiv + 1;
    }
     
    //Ya tenemos los tres índices de los puntos del triángulo en el que está la coordenada x,z
    //   Mediante la ecuación del plano, obtenemos la coordenada Y de nuestro punto x,z
	v1 = glm::vec3(m_Vertex[p1].x, m_Vertex[p1].y, m_Vertex[p1].z);
	v2 = glm::vec3(m_Vertex[p2].x, m_Vertex[p2].y, m_Vertex[p2].z);
	v3 = glm::vec3(m_Vertex[p3].x, m_Vertex[p3].y, m_Vertex[p3].z);
}

glm::vec3 CTerrain::getNormalAtPos(glm::vec3 pos)
{
    glm::vec3 v1, v2, v3;
	     
	/* Se calculan los puntos del triángulo en el que cae la pos */
    obtieneIndicesTriangulo(pos, v1, v2, v3);
             
    /* Obtenemos su normal */
	return calculaNormal(v1, v2, v3);
}

float CTerrain::getFronteraX()
{
	return m_iDivisiones * m_fAncho;
}

float CTerrain::getFronteraZ()
{
	return m_iDivisiones * m_fLargo;
}

Vertex3* CTerrain::getVertex()
{
	return m_Vertex;
}

Vertex3* CTerrain::getNormal()
{
	return m_Normal;
}

GLuint CTerrain::getNumVertex()
{
	return m_iNumVertex;
}

GLuint* CTerrain::getIndices()
{
	return m_Index;
}

float CTerrain::CalculaAlturaMax()
{
	float alt = 0.0;

	for (GLuint i = 0; i < m_iNumVertex; i++)
	{
		if (m_Vertex[i].y > alt)
		{
			alt = m_Vertex[i].y;
		}
	}

	return alt;
}

Vertex3 CTerrain::CalculaNormalVertice(GLuint iVertice)
{
	GLuint indices[6]; // 6 vértices adyacentes como máximo
	glm::vec3 vertices[6];
	glm::vec3 vector[7];
	glm::vec3 vec[6];
	GLuint nIndices = 0;
	GLuint nVertices = 0;
	GLuint nVectores = 0;
	Vertex3 resultado;
	glm::vec3 v0;
		
	//Inicializa la lista de índices
	for (GLuint i = 0; i < 6; i++)
		indices[i] = -1;
	
	//Obtiene el punto origen
	v0.x = m_Vertex[iVertice].x;
	v0.y = m_Vertex[iVertice].y;
	v0.z = m_Vertex[iVertice].z;
	
	//Según el número de divisiones y su posición se obtienen los vértices adyacentes
	if ( iVertice == 0 )
	{
		indices[0] = 1;
		indices[1] = m_iDivisiones + 1;
		indices[2] = m_iDivisiones;
		nIndices = 3;
	}
	else if ( iVertice > 0 && iVertice < m_iDivisiones - 1 )
	{
		indices[0] = iVertice + 1;
		indices[1] = iVertice + m_iDivisiones + 1;
		indices[2] = iVertice + m_iDivisiones;
		indices[3] = iVertice - 1;
		nIndices = 4;
	}
	else if ( iVertice == m_iDivisiones - 1 )
	{
		indices[0] = iVertice + m_iDivisiones;
		indices[1] = iVertice - 1;
		nIndices = 2;
	}
	else if ( (iVertice > 0) && (iVertice % m_iDivisiones == 0) && (iVertice < m_iDivisiones * (m_iDivisiones - 1)) )
	{
		indices[0] = iVertice - m_iDivisiones;
		indices[1] = iVertice + 1;
		indices[2] = iVertice + m_iDivisiones + 1;
		indices[3] = iVertice + m_iDivisiones;
		nIndices = 4;
	}
	else if ( (iVertice > m_iDivisiones) && ((iVertice + 1) % m_iDivisiones == 0) && (iVertice < (m_iDivisiones*m_iDivisiones - 1)) )
	{
		indices[0] = iVertice + m_iDivisiones;
		indices[1] = iVertice - 1;
		indices[2] = iVertice - m_iDivisiones - 1;
		indices[3] = iVertice - m_iDivisiones;
		nIndices = 4;
	}
	else if ( iVertice == m_iDivisiones * (m_iDivisiones - 1) )
	{
		indices[0] = iVertice - m_iDivisiones;
		indices[1] = iVertice + 1;
		nIndices = 2;
	}
	else if ( iVertice > m_iDivisiones * (m_iDivisiones - 1) && iVertice < m_iDivisiones * m_iDivisiones - 1 )
	{
		indices[0] = iVertice - 1;
		indices[1] = iVertice - m_iDivisiones - 1;
		indices[2] = iVertice - m_iDivisiones;
		indices[3] = iVertice + 1;
		nIndices = 4;
	}
	else if ( iVertice == m_iDivisiones * m_iDivisiones - 1 )
	{
		indices[0] = iVertice - 1;
		indices[1] = iVertice - m_iDivisiones - 1;
		indices[2] = iVertice - m_iDivisiones;
		nIndices = 3;
	}
	else
	{
		indices[0] = iVertice + 1;
		indices[1] = iVertice + m_iDivisiones + 1;
		indices[2] = iVertice + m_iDivisiones;
		indices[3] = iVertice - 1;
		indices[4] = iVertice - m_iDivisiones - 1;
		indices[5] = iVertice - m_iDivisiones;
		nIndices = 6;
	}

	//Obtiene las coordenadas de los vértices con los índices calculados
	// los guarda en la lista 'vertices'
	for (GLuint i = 0; i < nIndices; i++)
	{
		//Comprueba si está dentro de la rejilla del terreno
		if ( indices[i] >= 0 )
		{
			vertices[nVertices].x = m_Vertex[indices[i]].x;
			vertices[nVertices].y = m_Vertex[indices[i]].y;
			vertices[nVertices].z = m_Vertex[indices[i]].z;

			// Siguiente vértice
			nVertices++;
		}
	}

	if ( nVertices >= 2)
	{
		vector[0] = v0;
		for (GLuint i = 1; i <= nVertices; i++)
		{
			vector[i] = vertices[i-1];
		}

		nVectores = nVertices;
	}

	for (GLuint j = 0; j<nVectores; j++)
	{
		vec[j] = vector[j + 1] - vector[0];
	}

	GLuint l = 0;
	glm::vec3 dot[6];
	for (GLuint k = 0; k < nVectores - 1; k++)
	{
		dot[l] = glm::normalize(glm::cross(vec[k], vec[k + 1]));
		
		l++;
	}

	//En caso de ser un punto interior, tenemos 6 puntos cercanos, hay que 
	// tener en cuenta 6 caras para calcular la normal.
	if (nVectores == 6)
	{
		dot[l] = glm::normalize(glm::cross(vec[5], vec[0]));
	}

	glm::vec3 normal;
	for (GLuint m = 0; m < l; m++)
	{
		normal = normal + dot[m];
	}

	normal = glm::normalize(normal);

	resultado.x = normal.x;
	resultado.y = normal.y;
	resultado.z = normal.z;

	return resultado;
}

void CTerrain::Update()
{
	*m_WorldTransformation = m_camera->m_M4MVP;
}

void CTerrain::Animate(double dt)
{
	/* Animate the camera */
	m_camera->position.y = GetAltura(m_camera->position.x, m_camera->position.z);
	m_camera->position.y += 5.0;
}

void CTerrain::setLights(CLight *lights)
{
	//m_lights = lights;
}

/*void CTerrain::OnCollision(CObject *collisionobject)
{
	//Tratamos la colisión con el player
	if (collisionobject->m_idObjeto == PLAYER) 
	{
		getCamera()->position.y = GetAltura(collisionobject->m_CVPosition.x, collisionobject->m_CVPosition.z);
		getCamera()->position.y += 5.0f;

		//Comprobamos que la camara no sale del terreno por x o z
		//coordenada X
		if (collisionobject->m_CVPosition.x < 0)
			getCamera()->position.x = 2.0;
		else if (collisionobject->m_CVPosition.x > getFronteraX())
			getCamera()->position.x = getFronteraX();

		//coordenada Z
		if (collisionobject->m_CVPosition.z < 0)
			getCamera()->position.z = 2.0;
		else if (collisionobject->m_CVPosition.z > getFronteraZ())
			getCamera()->position.z = getFronteraZ();
	}
	else if (collisionobject->m_idObjeto == MONSTER)
	{
		//Calculamos la altura del objeto según su posición en el terreno
		collisionobject->m_CVPosition.y = GetAltura(collisionobject->m_CVPosition.x, collisionobject->m_CVPosition.z);
	}
}*/