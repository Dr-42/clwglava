#include "AudioCapture.h"
#include <system_error>

#define SAFE_RELEASE(punk)  \
if ((punk) != NULL)  \
{ (punk)->Release(); (punk) = NULL; }


void AudioCapture::winfatal_error(HRESULT hr, const char* fmt, ...) {
	if (FAILED(hr)) {
		std::string message = std::system_category().message(hr);
		va_list args;
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);
		fprintf(stderr, "Error: %s\n", message.c_str());
		this->~AudioCapture();
	}
}

AudioCapture::AudioCapture(){
	HRESULT hr = CoInitialize(NULL);
	winfatal_error(hr, "CoInitialize failed: hr = 0x%08x\n", hr);
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(void**)&pEnumerator);
	winfatal_error(hr, "CoCreateInstance failed: hr = 0x%08x\n", hr);

	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	winfatal_error(hr, "IMMDeviceEnumerator::GetDefaultAudioEndpoint failed: hr = 0x%08x\n", hr);
	// Initialize the a capture stream on the default audio device.
	LPWSTR name;
	hr = pDevice->GetId(&name);
	const auto iid_IAudioClient = __uuidof(IAudioClient);
	hr = pDevice->Activate(
		iid_IAudioClient, CLSCTX_ALL,
		NULL, (void**)&pAudioClient);
	winfatal_error(hr, "IMMDevice::Activate failed: hr = 0x%08x\n", hr);

	// Volume
	const auto iid_IAudioEndpointVolume = __uuidof(IAudioEndpointVolume);
	hr = pDevice->Activate(
		iid_IAudioEndpointVolume, CLSCTX_ALL, NULL,
		(void**)&pEndpointVolume);
	winfatal_error(hr, "IAudioClient::GetService failed: hr = 0x%08x\n", hr);

	hr = pAudioClient->GetMixFormat(&pwfx);
	winfatal_error(hr, "IAudioClient::GetMixFormat failed: hr = 0x%08x\n", hr);

	hr = pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		AUDCLNT_STREAMFLAGS_LOOPBACK,
		hnsRequestedDuration,
		0,
		pwfx,
		NULL);

	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);

	// Get a capture client
	const auto iid_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
	hr = pAudioClient->GetService(
		iid_IAudioCaptureClient,
		(void**)&pCaptureClient);
	winfatal_error(hr, "IAudioClient::GetService failed: hr = 0x%08x\n", hr);

	// Deduce the type of the PCM data
	wBitsPerSample = pwfx->wBitsPerSample;
	wChannels = pwfx->nChannels;

	// Start recording
	hr = pAudioClient->Start();
	winfatal_error(hr, "IAudioClient::Start failed: hr = 0x%08x\n", hr);
	data_length = bufferFrameCount * wChannels;
	data = new float[data_length];
}

uint64_t AudioCapture::fill_buffer(){
	HRESULT hr = pCaptureClient->GetBuffer(
		&pData,
		&numFramesAvailable,
		&flags, NULL, NULL);

	if (numFramesAvailable == 0) {
		// Sleep for half the buffer duration.
		Sleep((DWORD)(hnsRequestedDuration / REFTIMES_PER_MILLISEC / 50));
		return 0;
	}
	// Get the volume
	winfatal_error(hr, "IAudioCaptureClient::GetBuffer failed: hr = 0x%08x\n", hr);
	hr = pEndpointVolume->GetMasterVolumeLevelScalar(&volume);
	memcpy(data, pData, numFramesAvailable * wBitsPerSample / 8 * wChannels);
	hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
	winfatal_error(hr, "IAudioCaptureClient::ReleaseBuffer failed: hr = 0x%08x\n", hr);
	hr = pCaptureClient->GetNextPacketSize(&numFramesAvailable);
	winfatal_error(hr, "IAudioCaptureClient::GetNextPacketSize failed: hr = 0x%08x\n", hr);
	return numFramesAvailable * wChannels;
}

AudioCapture::~AudioCapture(){
	HRESULT hr = pAudioClient->Stop();  // Stop recording.
	winfatal_error(hr, "IAudioClient::Stop failed: hr = 0x%08x\n", hr);
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator)
	SAFE_RELEASE(pDevice)
	SAFE_RELEASE(pAudioClient)
	SAFE_RELEASE(pCaptureClient)
	CoUninitialize();
}

float* AudioCapture::get_data(){
	return data;
}
