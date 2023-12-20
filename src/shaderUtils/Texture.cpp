#include "shaderUtils/Texture.h"

#include <GL/glew.h>
Texture::Texture(int32_t width, int32_t height, uint8_t* data){
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
		 0,
		 GL_RGBA32F,
		 width,
		 height,
		 0,
		 GL_RGBA,
		 GL_UNSIGNED_BYTE,
	     data);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->id = texture;
}

Texture::~Texture(){
    glDeleteTextures(1, &this->id);
}


void Texture::Bind(uint32_t bindPoint){
    glBindImageTexture(bindPoint, this->id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}
