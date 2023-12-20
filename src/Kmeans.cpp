#include "Kmeans.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <ctime>

Kmeans::Kmeans(std::string imagePath, uint32_t iters){
    this->iters = iters;
    this->numCentroids = 10;
    srand(time(NULL));
    // Initialize data
    int channels;
    this->data = stbi_load(imagePath.c_str(), &this->width, &this->height, &channels, 4);
    const int size = width * height;

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    std::string computeFile = "assets/compute.glsl";
    computeShader = new ComputeShader(computeFile);
    texture = new Texture(width, height, data);
    counts = new Atomic(this->numCentroids);
    newCentroids = new Atomic(this->numCentroids * 3);
    centroids = new SSBO(this->numCentroids * 3);

    uint32_t* countsData = new uint32_t[this->numCentroids];
    uint32_t* newCentroidsData = new uint32_t[this->numCentroids * 3];
    this->centroidData = new uint32_t[this->numCentroids * 3];

    memset(countsData, 0, this->numCentroids * sizeof(uint32_t));
    memset(newCentroidsData, 0, this->numCentroids * 3 * sizeof(uint32_t));
    for (size_t i = 0; i < this->numCentroids; i++) {
        size_t randIndex = rand() % size;
        this->centroidData[i * 3] = data[randIndex * 4];
        this->centroidData[i * 3 + 1] = data[randIndex * 4 + 1];
        this->centroidData[i * 3 + 2] = data[randIndex * 4 + 2];
	}
    this->counts->SetData(this->numCentroids, countsData);
    this->newCentroids->SetData(this->numCentroids * 3, newCentroidsData);
    this->centroids->SetData(this->numCentroids * 3, this->centroidData);
    delete [] countsData;
    delete [] newCentroidsData;
} 

Kmeans::~Kmeans(){
    delete this->computeShader;
    delete this->texture;
    delete this->counts;
    delete this->newCentroids;
    delete this->centroids;
    delete [] this->centroidData;
    stbi_image_free(this->data);
}

std::vector<Color> Kmeans::GetColors(){
    for (size_t i = 0; i < this->iters; i++) {
        this->computeShader->Use();
        this->texture->Bind(0);
        this->counts->Bind(1);
        this->centroids->Bind(2);
        this->newCentroids->Bind(3);
        this->computeShader->Dispatch(this->width, this->height, 1);

        uint32_t* countsData = new uint32_t[this->numCentroids];
        uint32_t* newCentroidsData = new uint32_t[this->numCentroids * 3];

        this->counts->GetData(this->numCentroids, countsData);
        this->newCentroids->GetData(this->numCentroids * 3, newCentroidsData);

		for (size_t i = 0; i < this->numCentroids; i++) {
			if (countsData[i] == 0) {
				size_t rand_index = rand() % (this->width * this->height);
				this->centroidData[i * 3 + 0] = data[rand_index * 4 + 0];
				this->centroidData[i * 3 + 1] = data[rand_index * 4 + 1];
				this->centroidData[i * 3 + 2] = data[rand_index * 4 + 2];
			}
			else {
				this->centroidData[i * 3 + 0] = newCentroidsData[i * 3 + 0] / countsData[i];
				this->centroidData[i * 3 + 1] = newCentroidsData[i * 3 + 1] / countsData[i];
				this->centroidData[i * 3 + 2] = newCentroidsData[i * 3 + 2] / countsData[i];
			}
		}

        this->centroids->SetData(this->numCentroids * 3, this->centroidData);
        memset(newCentroidsData, 0, this->numCentroids * 3 * sizeof(uint32_t));
        this->newCentroids->SetData(this->numCentroids * 3, newCentroidsData);
        memset(countsData, 0, this->numCentroids * sizeof(uint32_t));
        this->counts->SetData(this->numCentroids, countsData);
        delete [] countsData;
        delete [] newCentroidsData;
	}

    std::vector<Color> colors;
    for (size_t i = 0; i < this->numCentroids; i++) {
        Color col = {0};
		col.r = (float)this->centroidData[i * 3 + 0] / 256.0f;
        col.g = (float)this->centroidData[i * 3 + 1] / 256.0f;
        col.b = (float)this->centroidData[i * 3 + 2] / 256.0f;
        col.a = 1.0f;
        colors.push_back(col);
	}
	return colors;
}

uint32_t Kmeans::GetNumColors()
{
	return 10;
}
