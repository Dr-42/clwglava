#pragma once

#include <cstdint>
#include <string>

class ComputeShader{
public:
	ComputeShader(std::string filename);
	~ComputeShader();
	void Use();
	void Dispatch(uint32_t x, uint32_t y, uint32_t z);
private:
	uint32_t id;
};

