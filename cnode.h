#ifndef _CNODE_H_
#define _CNODE_H_

#include <iostream>
#include <vector>

#include "cobject.h"

class CNode //: public CObject
{
public:
	/* Array of Objects one for each level in the hierarchy */
	std::vector<CObject*> m_Objects;
	bool m_IsVisible;

public:
	CNode();
	~CNode();

	/* Methods for Render, Update all the hierarchy level */
	void Animate(double dt);
	void Update();
	void Render();
};

#endif
