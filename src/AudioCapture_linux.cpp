#ifdef __linux__
#include "AudioCapture.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

AudioCapture::AudioCapture() {
    pa_simple *s = nullptr;
    int error;

    // Set up sample spec
    sample_spec.format = PA_SAMPLE_FLOAT32LE;
    sample_spec.channels = 2;
    sample_spec.rate = 44100;

    // Create a new playback stream
    s = pa_simple_new(
        NULL,
        "AudioCapture",
        PA_STREAM_RECORD,
        "bluez_output.C8_31_DB_58_26_65.1.monitor",
        "Record",
        &sample_spec,
        NULL,
        NULL,
        &error
    );

    if (!s) {
        fprintf(stderr, "pa_simple_new() failed: %s\n", pa_strerror(error));
        return;
    }

    pa_simple_ = s;
    data_length = sample_spec.channels * 512; // Adjust the buffer size as needed
    data = new float[data_length];
    volume = 1.0;
}

uint64_t AudioCapture::fill_buffer() {
    int error;
    ssize_t r;

    // Read audio data
    r = pa_simple_read(pa_simple_, data, data_length * sizeof(float), &error);

    if (r < 0) {
        fprintf(stderr, "pa_simple_read() failed: %s\n", pa_strerror(error));
        return 0;
    }
    for (size_t i = 0; i < data_length; i++) {
        data[i] *= 100.0f;
    }
    return data_length;
}

float* AudioCapture::get_data() {
    return data;
}

AudioCapture::~AudioCapture() {
    if (pa_simple_) {
        pa_simple_free(pa_simple_);
    }

    delete[] data;
}
#endif
