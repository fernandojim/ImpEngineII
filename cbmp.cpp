#include "stdafx.h"

#include "cbmp.h"
#include "utils.h"

CBmp::CBmp()
{
     m_pArchivo   = NULL;
     m_pPixelData = NULL;
     m_lWidth     = 0;
     m_lHeight    = 0;
	 m_bOk		  = false;
}

CBmp::CBmp(const char* file)
{
	m_bOk = loadBMP(file);
}

CBmp::~CBmp()
{
	if (m_pPixelData)
	{
		delete[] m_pPixelData;
		m_pPixelData = NULL;
	}
}

bool CBmp::loadBMP(const char *file)
{
	errno_t err;

    if (file == NULL)
       return false;
    
	m_pArchivo = fopen(file, "rb");
	 
	if (m_pArchivo)
	{
        //Leemos cabezera archivo bmp
        fread(&m_sBmfh, sizeof(BITMAPFILEHEADER), 1, m_pArchivo);
         
        //El campo bfType de un bmp será siempre 19778
        if (m_sBmfh.bfType == 19778)
        {
            //Leemos la cabecera de información
            fread(&m_sBmih, sizeof(BITMAPINFOHEADER), 1, m_pArchivo);
             
            //Ancho y largo de la textura
             m_lWidth  = m_sBmih.biWidth;
             m_lHeight = m_sBmih.biHeight;
             
             //Calculamos el tamaño de la información del mapa de colores
             m_dSize = m_sBmfh.bfSize - m_sBmfh.bfOffBits;
             
             //Reservamos memoria para la información del mapa de colores
			 BYTE *datos_aux = new BYTE[m_dSize];
			 m_pPixelData = new BYTE[m_dSize];
			 
			 //Leemos en bloque todo el mapa de pixeles
			 fread(datos_aux, sizeof(BYTE), m_dSize, m_pArchivo);

			 //calcula la posición en el paquete de datos de pixeles donde hay que aplicar 
			 //	el alineamiento a DWORD, es decir, mútiplos de 4 bytes.
			 //
			 //P.e si el ancho son 10 pixeles, serían 10 * 3 bytes = 30 bytes. Y el alineamiento
			 // a grupos de 4 bytes nos dejaría 7 palabras de 4 bytes y un resto de 2 bytes (align).
			 int align;
			 for (align = 0;align<4;align++)
			 {
				 if ( ((m_lWidth * 3) + align) % 4 == 0 )
					break;
			 }
			 
			 int j = 0;
			 int k = 0;
			 //Carga los datos de los bytes
			 for (GLuint i = 0; i < m_dSize; i++)
			 {
				 //Si llega al final de cada linea de datos aplica el alineamiento calculado
				 if ( (i > 1) && (i % ( m_lWidth * 3) == 0) )
					 k = k + align;
			
				 m_pPixelData[j] = datos_aux[k];

				 k++;
				 j++;
			}
		}
		else
		{
			fclose(m_pArchivo);
			
			return false;
		}
    }
	else
	{
		return false;
	}

    fclose(m_pArchivo);
    return true;
}
