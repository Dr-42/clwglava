#pragma once

#include <cstdint>

class SSBO {
public:
	SSBO(uint32_t size);
	~SSBO();

	void Bind(unsigned int bindPoint);
	void SetData(unsigned int size, void* data);
	void GetData(unsigned int size, void* data);
private:
	uint32_t id;
};
