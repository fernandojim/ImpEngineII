#include "stdafx.h"

#include "cnode.h"

CNode::CNode()
{
	/* Inits the object list */
	m_Objects.clear();

	/* Is visible by default */
	m_IsVisible = true;
}

/*
	Animate the objects in the node
*/
void CNode::Animate(double dt)
{
	for (GLuint i = 0; i < m_Objects.size(); i++)
	{
		if ((CObject*)m_Objects.at(i))
		{
			((CObject*)m_Objects.at(i))->Animate(dt);
		}
	}
}

/*
	Update the objects in the node
*/
void CNode::Update()
{
	for (GLuint i = 0; i < m_Objects.size(); i++)
	{
		if ((CObject*)m_Objects.at(i))
		{
			((CObject*)m_Objects.at(i))->Update();
		}
	}
}

/*
	Render the objects in the node
*/
void CNode::Render()
{
	for (GLuint i = 0; i < m_Objects.size(); i++)
	{
		if ((CObject*)m_Objects.at(i))
		{
			((CObject*)m_Objects.at(i))->Render();
		}
	}
}

/*
	Destructor
*/
CNode::~CNode()
{
	m_IsVisible = false;

	for (GLuint i = 0; i < m_Objects.size(); i++)
		delete m_Objects[i];
}