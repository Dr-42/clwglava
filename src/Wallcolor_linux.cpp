#ifdef __linux__

#include <cstring>
#include <pwd.h>
#include <unistd.h>

#include "Wallcolor.h"
#include "Kmeans.h"

std::string get_wallpaper_path() {
	// Get wallpaper path KDE
	const char *homedir;

	if ((homedir = getenv("HOME")) == NULL) {
		homedir = getpwuid(getuid())->pw_dir;
	}
	const char *configdir = "/.config/plasma-org.kde.plasma.desktop-appletsrc";
	char *path = (char *)malloc(strlen(homedir) + strlen(configdir) + 1);
	strcpy(path, homedir);
	strcat(path, configdir);

	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", path);
		free(path);
	}
	free(path);
	
	std::string wallpaperPath;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, fp)) != -1) {
		if (strncmp(line, "Image=file://", 13) == 0) {
			wallpaperPath = std::string(line + 13, read - 14);
			break;
		}
	}
	free(line);
	fclose(fp);
	return wallpaperPath;
}

Wallcolor::Wallcolor(){
	wallpaperPath = get_wallpaper_path();
	printf("Wallpaper path: %s\n", wallpaperPath.c_str());
}

Wallcolor::~Wallcolor(){
}

bool Wallcolor::checkChange(){
	std::string newWallpaperPath = get_wallpaper_path();
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
