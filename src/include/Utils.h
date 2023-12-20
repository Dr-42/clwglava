#pragma once
#include "Windows.h"
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
	static void winfatal_error(HRESULT hr, const char* fmt, ...);
	static std::string wstring_to_string(const std::wstring& wstr);
};

