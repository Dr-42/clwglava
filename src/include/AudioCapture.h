#pragma once

#ifdef _WIN64
#include <initguid.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <winnt.h>
#include <endpointvolume.h>
#include <audioclient.h>
#endif

#ifdef __linux__
#include <alsa/asoundlib.h>
#endif

#include <cstdint>
#include <stdio.h>

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
	#ifdef _WIN64
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
	#endif
	#ifdef __linux__
    snd_pcm_t *pcmHandle;
    snd_pcm_uframes_t bufferFrameCount;
    size_t wBitsPerSample;
    size_t wChannels;
	#endif
};
