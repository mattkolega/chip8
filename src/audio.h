#ifndef CHIP_8_AUDIO_H
#define CHIP_8_AUDIO_H

#include "miniaudio.h"

typedef struct AudioContext {
    ma_device *device;
    ma_waveform *squareWave;
} AudioContext;

// Initialise audio device and waveform
AudioContext initAudio();

// Start playback of waveform
void startPlayback(ma_device *device);

// Stop playback of waveform
void stopPlayback(ma_device *device);

// Un-init audio device and free context memory
void destroyAudio(AudioContext audioContext);

#endif