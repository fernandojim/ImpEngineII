#include "stdafx.h"

#include "cterrain.h"

using namespace MaterialManager;

#define PUNTOS_ALREDEDOR 6

//------------------------------------------
// Constructor by file
//------------------------------------------
CTerrain::CTerrain(const string file) : CGameObject()
{
	GLuint x, z, k;
	float escala = 0.0;
	float rgb_max = 0.0;
	glm::vec3 vertex;
	glm::vec2 texel;
	glm::vec3 normal;
	GLuint index;
	GLuint id_buffer = 0;

	// Create the File Object
	m_File = new CFileDef(file);

	// Read the content of file
	if (m_File->getIsOpen())
	{
		m_File->readObjectKeysValues();

		/* Gets the object id */
		m_iId = getObjectId();

		/* Gets the properties */
		m_fAncho = m_File->getFloatObjectValues("ancho")[0];
		m_fLargo = m_File->getFloatObjectValues("largo")[0];
		m_fAlturaMax = m_File->getFloatObjectValues("altura")[0];
		m_texturaHeightfield = m_File->getStringObjectValues("heightfield")[0];
		m_materialsName = m_File->getStringObjectValues("materials")[0];

		m_tilingFactorSand = 1.0;
		m_tilingFactorGrass = 1.0;
		m_tilingFactorDirt = 1.0;
		m_tilingFactorSnow = 1.0;

		/* Load the materials from file */
		::getMaterialManager().loadMaterialsFromMTL(m_materialsName);

		delete m_File;

		/* Store the materials loadad into terrain data */
		m_MatSand = *::getMaterialManager().getMaterialByName(string("sand"));
		m_MatGrass = *::getMaterialManager().getMaterialByName(string("grass"));
		m_MatDirt = *::getMaterialManager().getMaterialByName(string("dirt"));
		m_MatSnow = *::getMaterialManager().getMaterialByName(string("snow"));

		//Carga la imagen del mapa de alturas
		m_pHeightMap = new CBmp(m_texturaHeightfield.c_str());

		//Si la textura es válida, toma el ancho de esta como m_iDivisiones
		if (m_pHeightMap != NULL)
			m_iDivisiones = m_pHeightMap->m_lHeight;

		//Calcula el número total de vértices y crea la lista
		m_iNumVertex = m_iDivisiones * m_iDivisiones;

		//Calcula el número total de indices de vértices y crea la lista
		m_iNumIndex = (m_iDivisiones * 2) * (m_iDivisiones - 1);

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
				vertex.x = x * m_fAncho;
				vertex.y = m_pHeightMap->m_pPixelData[k++ * 3] * escala;
				vertex.z = z * m_fLargo;

				m_Vertex.push_back(vertex);

				texel.x = static_cast<float>(x);// / static_cast<float>(m_iDivisiones);
				texel.y = static_cast<float>(z);// / static_cast<float>(m_iDivisiones);

				m_Texel.push_back(texel);
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
		for (x = 0; x < m_iDivisiones - 1; x++)
		{
			for (z = 0; z < m_iDivisiones; z++)
			{
				index = (m_iDivisiones * (x + 1)) + z;
				m_Index.push_back(index);

				index = (m_iDivisiones *  x) + z;
				m_Index.push_back(index);
			}
		}

		//Después de crear los vértices del terreno, calculo las normales
		for (GLuint cont = 0; cont < m_iNumVertex; cont++)
		{
			normal = CalculaNormalVertice(cont);
			m_Normal.push_back(normal);
		}

		//Crea los buffers del objeto VAO
		id_buffer = CreateArrayBuffer(&m_Vertex[0], m_iNumVertex * sizeof(float) * 3, GL_STATIC_DRAW);
		CreateAttribArrayBuffer(0, id_buffer, 3);

		id_buffer = CreateArrayBuffer(&m_Normal[0], m_iNumVertex * sizeof(float) * 3, GL_STATIC_DRAW);
		CreateAttribArrayBuffer(1, id_buffer, 3);

		id_buffer = CreateArrayBuffer(&m_Texel[0], m_iNumVertex * sizeof(float) * 2, GL_STATIC_DRAW);
		CreateAttribArrayBuffer(2, id_buffer, 2);

		//The terrain has no parents.
		m_pParent = NULL;

		//After all values is loaded, mark the object terrain as valid
		m_bIsValid = true;
	}
	else
		m_bIsValid = false;
}

CTerrain::~CTerrain()
{
	delete m_pHeightMap;
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

	/* Creamos los vectores según los puntos introducidos */
	vector1 = p2 - p1;
    vector2 = p3 - p1;

	/* El producto vectorial entre dos vectores es la normal al plano que forman */
	return glm::normalize(glm::cross(vector1, vector2));
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

glm::vec3 CTerrain::CalculaNormalVertice(GLuint iVertice)
{
	GLuint indices[6]; // 6 vértices adyacentes como máximo
	glm::vec3 vertices[6];
	glm::vec3 vector[7];
	glm::vec3 vec[6];
	GLuint nIndices = 0;
	GLuint nVertices = 0;
	GLuint nVectores = 0;
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

	return normal;
}

void CTerrain::setLights(CLight *lights)
{
	//m_lights = lights;
}
