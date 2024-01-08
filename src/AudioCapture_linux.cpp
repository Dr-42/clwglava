#ifdef __linux__
#include "AudioCapture.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

AudioCapture::AudioCapture() {
    pa_simple *s = nullptr;
    int error;

    // Set up sample spec
    sample_spec.format = PA_SAMPLE_FLOAT32LE;
    sample_spec.channels = 2;
    sample_spec.rate = 44100;

    std::string device = exec("pactl get-default-sink");
    device += ".monitor";

    // Create a new playback stream
    s = pa_simple_new(
        NULL,
        "AudioCapture",
        PA_STREAM_RECORD,
        device.c_str(),
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
    data_length = sample_spec.channels * 996; // Adjust the buffer size as needed
    data = new float[data_length];
    volume = 1.0;
}

uint64_t AudioCapture::fill_buffer() {
    int error = 0;
    ssize_t r;

    // Read audio data
    //r = pa_simple_read(pa_simple_, data, data_length * sizeof(float), &error);
    if (read_future.valid()) {
        if (read_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
            r = read_future.get();
            if (r < 0) {
                fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
                return 0;
            }
            return data_length;
        } else {
            return 0;
        }
    } else {
        read_future = std::async(std::launch::async, &pa_simple_read, pa_simple_, data, data_length * sizeof(float), &error);
        return 0;
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
