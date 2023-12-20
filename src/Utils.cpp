#include "Utils.h"

#include <stdio.h>
#include <string>
#include <system_error>

void Utils::winfatal_error(HRESULT hr, const char* fmt, ...) {
	if (FAILED(hr)) {
		std::string message = std::system_category().message(hr);
		va_list args;
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);
		fprintf(stderr, "Error: %s\n", message.c_str());
	}
}

std::string Utils::wstring_to_string(const std::wstring& wstr){
	std::string str(wstr.begin(), wstr.end());
	return str;
}
