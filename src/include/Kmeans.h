#pragma once

#include <vector>
#include <string>
#include "Utils.h"
#include "shaderUtils/ComputeShader.h"
#include "shaderUtils/Atomic.h"
#include "shaderUtils/Ssbo.h"
#include "shaderUtils/Texture.h"

class Kmeans{
public:
	Kmeans(std::string imagePath, uint32_t iters);
	~Kmeans();
	std::vector<Color> GetColors();
	uint32_t GetNumColors();
private:
	int32_t width;
	int32_t height;
	uint32_t iters;
	std::vector<Color> colors;
	uint8_t* data;
	uint32_t* centroidData;
	uint32_t numCentroids;
	ComputeShader* computeShader;
	Texture* texture;
	SSBO* centroids;
	Atomic* counts;
	Atomic* newCentroids;
};

