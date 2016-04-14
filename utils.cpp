#include "stdafx.h"

#include "utils.h"

namespace ansiCToUnicode
{
	std::wstring ansiCToUnicode(char *z_c)
	{
		std::string aux = z_c;
		std::wstring stemp = std::wstring(aux.begin(), aux.end());
		std::wstring sw = stemp.c_str();

		return sw;
	}
}