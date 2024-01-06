#ifdef _WIN64
#include "Wallcolor.h"

#include "Kmeans.h"

Wallcolor::Wallcolor(){
	HRESULT hr = CoCreateInstance(__uuidof(DesktopWallpaper), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pDesktopWallpaper));
	Utils::winfatal_error(hr, "Cannot create wallpaper instance");
	hr = pDesktopWallpaper->GetMonitorDevicePathAt(0, &monitorDevicePath);
	Utils::winfatal_error(hr, "Cannot get monitor device path");
	PWSTR wallpaperPathW;
	hr = pDesktopWallpaper->GetWallpaper(monitorDevicePath, &wallpaperPathW);
	Utils::winfatal_error(hr, "Cannot get wallpaper path");
	wallpaperPath = Utils::wstring_to_string(wallpaperPathW);
}

Wallcolor::~Wallcolor(){
	// Free memory
	CoTaskMemFree(monitorDevicePath);
	pDesktopWallpaper->Release();
}

bool Wallcolor::checkChange(){
	PWSTR newWallpaperPathW;
	HRESULT hr = pDesktopWallpaper->GetWallpaper(monitorDevicePath, &newWallpaperPathW);
	Utils::winfatal_error(hr, "Cannot get wallpaper path");
	std::string newWallpaperPath = Utils::wstring_to_string(newWallpaperPathW);
	if (newWallpaperPath != wallpaperPath) {
		wallpaperPath = newWallpaperPath;
		return true;
	} else {
		return false;
	}
}

std::vector<Color> Wallcolor::getColors()
{
	Kmeans kmeans(wallpaperPath, 30);
	auto colors = kmeans.GetColors();
	return colors;
}
#endif
