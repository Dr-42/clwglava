#pragma once
#ifdef _WIN64
#include <Windows.h>
#endif
#include <string>

#define SAFE_RELEASE(punk)  \
if ((punk) != NULL)  \
{ (punk)->Release(); (punk) = NULL; }

struct Color {
    float r, g, b, a;
};


class Utils
{
public:
	#ifdef _WIN64
	static void winfatal_error(HRESULT hr, const char* fmt, ...);
	#endif
	static std::string wstring_to_string(const std::wstring& wstr);
};

