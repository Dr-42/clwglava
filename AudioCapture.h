#pragma once

#include <initguid.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <winnt.h>
#include <stdio.h>
#include <endpointvolume.h>
#include <audioclient.h>

#include <cstdint>

constexpr auto REFTIMES_PER_SEC = 10000000;
constexpr auto REFTIMES_PER_MILLISEC = 10000;

class AudioCapture {
public:
	AudioCapture();
	~AudioCapture();
	uint64_t fill_buffer();
	float* get_data();
	float volume;
private:
	uint64_t data_length;
	float* data;
	IMMDeviceEnumerator* pEnumerator = NULL;
	WAVEFORMATEX* pwfx = NULL;
	IMMDevice* pDevice = NULL;
	IAudioClient* pAudioClient = NULL;
	IAudioCaptureClient* pCaptureClient = NULL;
	BYTE* pData;
	IAudioEndpointVolume* pEndpointVolume = NULL;
	DWORD flags;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	WORD wBitsPerSample;
	WORD wChannels;

	void winfatal_error(HRESULT hr, const char* fmt, ...);
};
