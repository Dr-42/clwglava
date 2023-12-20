#include "shaderUtils/Ssbo.h"

#include <GL/glew.h>
#include <cstddef>

SSBO::SSBO(uint32_t size){
    uint32_t buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    this->id = buffer;
}

SSBO::~SSBO(){
    glDeleteBuffers(1, &this->id);
}

void SSBO::Bind(unsigned int bindPoint){
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindPoint, this->id);
}

void SSBO::SetData(unsigned int size, void* data){
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
}

void SSBO::GetData(unsigned int size, void* data){
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->id);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
}

