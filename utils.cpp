#include "stdafx.h"

#include "utils.h"

namespace ansiCToUnicode
{
	std::wstring ansiCToUnicode(const char *z_c)
	{
		std::string aux = z_c;
		std::wstring stemp = std::wstring(aux.begin(), aux.end());
		std::wstring sw = stemp.c_str();

		return sw;
	}
}

namespace Message
{
	void MessageBOX(const char* title, const char* message)
	{
#ifdef _VSTUDIO
		MessageBox(NULL, ansiCToUnicode::ansiCToUnicode(title).c_str(),
			ansiCToUnicode::ansiCToUnicode(message).c_str(), MB_OK);
#else   //Eclipse Mars2 5.11
		MessageBox(NULL, title, message, MB_OK);
#endif
		exit(0);
	}
}
