#include "shaderUtils/ComputeShader.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>

ComputeShader::ComputeShader(std::string filename){
    std::ifstream file(filename);
    std::string computeShader = {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    GLuint program = glCreateProgram();
    GLuint cShader = glCreateShader(GL_COMPUTE_SHADER);
    const char* cShaderText = computeShader.c_str();
    glShaderSource(cShader, 1, &cShaderText, NULL);
    glCompileShader(cShader);
    GLint success;
    glGetShaderiv(cShader, GL_COMPILE_STATUS, &success);
    if (!success) {
	GLchar infoLog[2048];
	glGetShaderInfoLog(cShader, 2048, NULL, infoLog);
	std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
	exit(1);
    }
    glAttachShader(program, cShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
	GLchar infoLog[2048];
	glGetProgramInfoLog(program, 2048, NULL, infoLog);
	std::cerr << "ERROR::SHADER::COMPUTE::LINKING_FAILED\n" << infoLog << std::endl;
	exit(1);
    }
    glDeleteShader(cShader);
    this->id = program;
}

ComputeShader::~ComputeShader(){
    glDeleteProgram(this->id);
}

void ComputeShader::Use(){
    glUseProgram(this->id);
}

void ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z){
    glDispatchCompute(x, y, z);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}
