#include "shaderUtils/Atomic.h"

#include <GL/glew.h>
#include <cstddef>

Atomic::Atomic(uint32_t size){
    uint32_t buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buffer);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, size * sizeof(uint32_t), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    this->id = buffer;
}

Atomic::~Atomic(){
    glDeleteBuffers(1, &this->id);
}

void Atomic::Bind(unsigned int bindPoint){
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, bindPoint, this->id);
}

void Atomic::SetData(unsigned int size, void* data){
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, this->id);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, size * sizeof(uint32_t), data, GL_DYNAMIC_COPY);
}

void Atomic::GetData(unsigned int size, void* data){
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, this->id);
    glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, size * sizeof(uint32_t), data);
}

