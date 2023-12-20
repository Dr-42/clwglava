#pragma once

#include <cstdint>

class Atomic {
public:
	Atomic(uint32_t size);
	~Atomic();

	void Bind(unsigned int bindPoint);
	void SetData(unsigned int size, void* data);
	void GetData(unsigned int size, void* data);
private:
	uint32_t id;
};
