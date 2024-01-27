#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include "audio.h"

#include <SDL.h>

#define DEVICE_FORMAT       ma_format_f32
#define DEVICE_CHANNELS     2
#define DEVICE_SAMPLE_RATE  44100

const double WAVEFORM_AMPLITUDE = 0.2;
const double WAVEFORM_FREQUENCY = 200;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_waveform* pSquareWave;

    MA_ASSERT(pDevice->playback.channels == DEVICE_CHANNELS);

    pSquareWave = (ma_waveform*)pDevice->pUserData;
    MA_ASSERT(pSquareWave != NULL);

    ma_waveform_read_pcm_frames(pSquareWave, pOutput, frameCount, NULL);

    (void)pInput;  // Avoid unused variable warning
}

AudioContext initAudio() {
    AudioContext audioContext;
    ma_device_config deviceConfig;
    ma_waveform_config squareWaveConfig;

    audioContext.squareWave = malloc(sizeof(ma_waveform));
    audioContext.device = malloc(sizeof(ma_device));

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = DEVICE_FORMAT;
    deviceConfig.playback.channels = DEVICE_CHANNELS;
    deviceConfig.sampleRate        = DEVICE_SAMPLE_RATE;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = audioContext.squareWave;

    if (ma_device_init(NULL, &deviceConfig, audioContext.device) != MA_SUCCESS) {
        SDL_Log("Error: Failed to open playback device.");
    }

    SDL_Log("Audio Device Name: %s", audioContext.device->playback.name);

    squareWaveConfig = ma_waveform_config_init(audioContext.device->playback.format, audioContext.device->playback.channels, audioContext.device->sampleRate, ma_waveform_type_square, WAVEFORM_AMPLITUDE, WAVEFORM_FREQUENCY);
    ma_waveform_init(&squareWaveConfig, audioContext.squareWave);

    return audioContext;
}

void startPlayback(ma_device *device) {
    if (ma_device_start(device) != MA_SUCCESS) SDL_Log("Error: Failed to start playback device.");
}

void stopPlayback(ma_device *device) {
    if (ma_device_stop(device) != MA_SUCCESS) SDL_Log("Error: Failed to stop playback device.");
}

void destroyAudio(AudioContext audioContext) {
    ma_device_uninit(audioContext.device);
    free(audioContext.device);
    free(audioContext.squareWave);
}