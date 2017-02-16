#include "stdafx.h"

#include "cbmp.h"

CBmp::CBmp(std::string file)
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

void CBmp::clear()
{
     m_pPixelData = NULL;
     m_lWidth     = 0;
     m_lHeight    = 0;
	 m_bOk		  = false;
	 m_dSize      = 0;
}

bool CBmp::loadBMP(std::string sfile)
{
	int align = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	ifstream file;

	file.open(sfile.c_str());

	if (file.is_open())
	{
        //Leemos cabezera archivo bmp
		file.seekg(0, std::ios::beg);
		file.read((char*)&m_sBmfh, sizeof(BITMAPFILEHEADER));

        //El campo bfType de un bmp será siempre 19778
        if (m_sBmfh.bfType == 19778)
        {
        	//Leemos la cabecera de información
        	file.read((char*)&m_sBmih, sizeof(BITMAPINFOHEADER));

            //Ancho y largo de la textura
            m_lWidth  = m_sBmih.biWidth;
            m_lHeight = m_sBmih.biHeight;
             
            //Calculamos el tamaño de la información del mapa de colores
            m_dSize = m_sBmfh.bfSize - m_sBmfh.bfOffBits;
             
            //Reservamos memoria para la información del mapa de colores
			BYTE *datos_aux = new BYTE[m_dSize];
			m_pPixelData = new BYTE[m_dSize];
			 
			//Leemos en el mapa de pixeles
			file.read((char*)&datos_aux[0], sizeof(BYTE) * m_dSize);

			//calcula la posición en el paquete de datos de pixeles donde hay que aplicar
			//	el alineamiento a DWORD, es decir, mútiplos de 4 bytes.
			//
			//P.e si el ancho son 10 pixeles, serían 10 * 3 bytes = 30 bytes. Y el alineamiento
			// a grupos de 4 bytes nos dejaría 7 palabras de 4 bytes y un resto de 2 bytes (align).
			for (align = 0;align<4;align++)
			{
				if ( ((m_lWidth * 3) + align) % 4 == 0 )
					break;
			}
			 
			//Carga los datos de los bytes
			for (i = 0; i < (int)m_dSize; i++)
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
			file.close();
			return false;
		}
    }
	else
	{
		return false;
	}

	file.close();
    return true;
}
