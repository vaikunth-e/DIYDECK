#ifndef REVERB_MODEL_H
#define REVERB_MODEL_H

#include <Arduino.h>

// mono reverb model.
// main.cpp should only call this interface; comb/allpass internals stay separate.
void reverbInit(uint32_t sampleRateHz);
int reverbProcessSample(int centeredSample);
void reverbSetWet(float wet);
void reverbSetDry(float dry);
void reverbSetRoomSize(float roomSize);
void reverbSetDamping(float damping);
void reverbMute();

#endif
