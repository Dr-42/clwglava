#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vector>

struct Rect {
    float x, y, w, h, rot;
};

struct Color {
    float r, g, b, a;
};

class Renderer {
   public:
    Renderer();
    ~Renderer();
    void Begin();
    void End();

    void LoadShader();

    void DrawRect(Rect rect, Color bcolor, Color tcolor);
    GLFWwindow* window;

   private:
    unsigned int shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<float> mVerts;
    std::vector<unsigned int> mIndices;
    uint32_t mNumRects;
};
