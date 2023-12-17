#pragma once
#include "Windows.h"

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
};

