#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>

namespace ansiCToUnicode
{
	std::wstring ansiCToUnicode(const char *z_c);
}

namespace Message
{
	void MessageBOX(const char* title, const char* message);
}

#endif