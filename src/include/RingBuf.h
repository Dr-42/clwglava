#pragma once

#include <complex>
#include <cstdint>

#define PI 3.14159265358979323846

class RingBuf {
public:
    RingBuf(unsigned int size);
    ~RingBuf();
    void Push(float val);
    void Cat(float* vals, uint64_t num);
    float Get(size_t index);
    unsigned int GetSize();
    unsigned int GetIndex();
    float operator[](size_t index);
	// Larger facter = lower fft_size
    void fft_analyze(uint32_t factor, float dt);
    float get_fft(size_t index);
    size_t fft_size;
    float max_amp;
private:
    float* buf;
    std::complex<float>* fft_buf;
    unsigned int size;
    int index;
    void fft(float in[], size_t stride, std::complex<float> out[], size_t n);
};

