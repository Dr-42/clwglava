#pragma once

#include <string>
#include <vector>
#include <Shobjidl.h>

#include "Utils.h"

class Wallcolor{
public:
	Wallcolor();
	~Wallcolor();
	bool checkChange();
	std::vector<Color> getColors();
private:
	std::string wallpaperPath;
	IDesktopWallpaper *pDesktopWallpaper;
	PWSTR monitorDevicePath;
};

