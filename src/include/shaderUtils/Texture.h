#pragma once

#include <cstdint>
class Texture {
public:
	Texture(int32_t width, int32_t height, uint8_t* data);
	~Texture();

	void Bind(uint32_t bindPoint);
private:
	uint32_t id;
};
