#pragma once

#include <string>
#include <vector>
#ifdef _WIN64
#include <Shobjidl.h>
#endif

#include "Utils.h"

class Wallcolor{
public:
	Wallcolor();
	~Wallcolor();
	bool checkChange();
	std::vector<Color> getColors();
private:
	std::string wallpaperPath;
	#ifdef _WIN64
	IDesktopWallpaper *pDesktopWallpaper;
	PWSTR monitorDevicePath;
	#endif
};

