#include "RingBuf.h"

#include <cmath>
#include <cstdint>


RingBuf::RingBuf(unsigned int size) {
    this->size = size;
    this->buf = new float[size];
    this->fft_buf = new std::complex<float>[size];
    // Set all values to 0
    for (unsigned int i = 0; i < size; i++) {
        this->buf[i] = 0;
        this->fft_buf[i] = 0;
    }
    this->index = 0;
}

RingBuf::~RingBuf() {
    delete[] this->buf;
    delete[] this->fft_buf;
}

void RingBuf::Push(float val) {
    this->buf[this->index] = val;
    this->index = (this->index + 1) % this->size;
}

void RingBuf::Cat(float* vals, int num) {
    for (int i = 0; i < num; i++) {
        this->Push(vals[i]);
    }
}

float RingBuf::Get(int index) {
    return this->buf[(this->index + index) % this->size];
}

unsigned int RingBuf::GetSize() {
    return this->size;
}

unsigned int RingBuf::GetIndex() {
    return this->index;
}

float RingBuf::operator[](int index) {
    return this->Get(index);
}

std::complex<float> mulcc(std::complex<float> a, std::complex<float> b) {
    return {
        std::real(a) * std::real(b) - std::imag(a) * std::imag(b),
        std::real(a) * std::imag(b) + std::imag(a) * std::real(b)
    };
}

std::complex<float> addcc(std::complex<float> a, std::complex<float> b) {
	return {
		std::real(a) + std::real(b),
		std::imag(a) + std::imag(b)
	};
}

std::complex<float> subcc(std::complex<float> a, std::complex<float> b) {
	return {
		std::real(a) - std::real(b),
		std::imag(a) - std::imag(b)
	};
}

void RingBuf::fft_analyze(uint32_t factor, float dt) {
    // Apply the Hann Window on the Input - https://en.wikipedia.org/wiki/Hann_function
    for (size_t i = 0; i < this->size; ++i) {
        float t = static_cast<float>(i) / (this->size - 1);
        float hann = 0.5 - 0.5 * std::cos(2 * PI * t);
        this->buf[i] = this->buf[i] * hann;
    }

    // FFT
    this->fft(this->buf, 1, this->fft_buf, this->size);

    // "Squash" into the Logarithmic Scale
    float step = 1.06;
    float lowf = 1.0f;
    size_t m = 0;
    float max_amp = 1.0f;
    for (float f = lowf; static_cast<size_t>(f) < this->size / factor; f = std::ceil(f * step)) {
        float f1 = std::ceil(f * step);
        float a = 0.0f;
        for (size_t q = static_cast<size_t>(f); q < this->size / factor && q < static_cast<size_t>(f1); ++q) {
            float b = std::abs(this->fft_buf[q]);
            if (b > a) a = b;
        }
        if (max_amp < a) max_amp = a;
        this->fft_buf[m++] = a;
    }

    // Normalize Frequencies to 0..1 range
    for (size_t i = 0; i < m; ++i) {
        this->fft_buf[i] /= max_amp;
    }

    // Smooth out and smear the values
    for (size_t i = 0; i < m; ++i) {
        float smoothness = 8;
        this->fft_buf[i] += (this->fft_buf[i] - this->fft_buf[i]) * smoothness * dt;
        float smearness = 3;
        this->fft_buf[i] += (this->fft_buf[i] - this->fft_buf[i]) * smearness * dt;
    }
	fft_size = m;
}

float RingBuf::get_fft(int index) {
    return std::abs(this->fft_buf[index]);
}

void RingBuf::fft(float in[], size_t stride, std::complex<float> out[], size_t n) {
    if (n == 1) {
        out[0] = in[0];
        return;
    }

    fft(in, stride * 2, out, n / 2);
    fft(in + stride, stride * 2, out + n / 2, n / 2);

    for (size_t k = 0; k < n / 2; ++k) {
        float t = static_cast<float>(k) / n;
        auto v = mulcc(std::exp(std::imag(-2 * PI * t)), out[k + n / 2]);
        auto e = out[k];
        out[k] = addcc(e, v);
        out[k + n / 2] = subcc(e, v);
    }
}
