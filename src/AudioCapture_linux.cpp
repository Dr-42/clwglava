#ifdef __linux__
#include "AudioCapture.h"
#include <cstdint>
#include <unistd.h>

#define ALSA_PCM_NEW_HW_PARAMS_API

AudioCapture::AudioCapture() {
    // Initialize ALSA
    snd_pcm_hw_params_t *params;
    snd_pcm_hw_params_alloca(&params);

    int err;
    if ((err = snd_pcm_open(&pcmHandle, "default", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "Cannot open PCM device: %s\n", snd_strerror(err));
        // Handle error
    }

    // Set parameters
    if ((err = snd_pcm_hw_params_any(pcmHandle, params)) < 0) {
        fprintf(stderr, "Cannot configure this PCM device: %s\n", snd_strerror(err));
        // Handle error
    }

    // Set desired parameters (you may need to adapt these based on your requirements)
    if ((err = snd_pcm_hw_params_set_access(pcmHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "Error setting interleaved mode: %s\n", snd_strerror(err));
        // Handle error
    }

    // Set format (assuming 16-bit signed little-endian)
    if ((err = snd_pcm_hw_params_set_format(pcmHandle, params, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf(stderr, "Error setting format: %s\n", snd_strerror(err));
        // Handle error
    }

    // Set channels
    if ((err = snd_pcm_hw_params_set_channels(pcmHandle, params, 2)) < 0) {
        fprintf(stderr, "Error setting channels: %s\n", snd_strerror(err));
        // Handle error
    }

    // Set rate (assuming 44100 Hz)
    unsigned int rate = 44100;
    if ((err = snd_pcm_hw_params_set_rate_near(pcmHandle, params, &rate, 0)) < 0) {
        fprintf(stderr, "Error setting rate: %s\n", snd_strerror(err));
        // Handle error
    }

    // Apply parameters
    if ((err = snd_pcm_hw_params(pcmHandle, params)) < 0) {
        fprintf(stderr, "Cannot set parameters: %s\n", snd_strerror(err));
        // Handle error
    }

    // Get buffer size
    snd_pcm_hw_params_get_period_size(params, &bufferFrameCount, 0);

    // Allocate memory for audio data
    data_length = bufferFrameCount * 2; // Assuming 2 channels
    data = new float[data_length];
}

uint64_t AudioCapture::fill_buffer() {
    ssize_t frames = snd_pcm_readi(pcmHandle, data, bufferFrameCount);
    if (frames < 0) {
        fprintf(stderr, "Error from read: %s\n", snd_strerror(frames));
        // Handle error
    }

    return static_cast<uint64_t>(frames);
}

AudioCapture::~AudioCapture() {
    // Release resources
    snd_pcm_close(pcmHandle);
    delete[] data;
}

float *AudioCapture::get_data() {
    return data;
}
#endif
