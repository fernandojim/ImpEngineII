/*#include "stdafx.h"

#include "cmesh.h"

CMesh::CMesh()
{
	m_iNumVertex = 0;
	m_iNumIndex = 0;

	m_Vertex = NULL;
	m_Normal = NULL;
	m_Texel = NULL;
	m_Index = NULL;
}

CMesh::~CMesh()
{
	m_iNumVertex = 0;
	m_iNumIndex = 0;

	if (m_Vertex)
	{
		delete[] m_Vertex;
		m_Vertex = NULL;
	}
	
	if (m_Normal)
	{
		delete[] m_Normal;
		m_Normal = NULL;
	}
	
	if (m_Texel)
	{
		delete[] m_Texel;
		m_Texel = NULL;
	}
	
	if (m_Index)
	{
		delete[] m_Index;
		m_Index = NULL;
	}
}*/