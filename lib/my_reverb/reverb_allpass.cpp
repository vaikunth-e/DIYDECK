#include "reverb_allpass.h"
#include "reverb_denormals.h"

void ReverbAllpass::setBuffer(float *bufferPtr, int size) {
  buffer = bufferPtr;
  bufferSize = size;
  index = 0;
  mute();
}

void ReverbAllpass::setFeedback(float value) {
  feedback = value;
}

void ReverbAllpass::mute() {
  if (!buffer) return;

  for (int i = 0; i < bufferSize; i++) {
    buffer[i] = 0.0f;
  }

  index = 0;
}

float ReverbAllpass::process(float input) {
  float buffered = undenormalize(buffer[index]);
  float output = -input + buffered;

  buffer[index] = input + (buffered * feedback);

  index++;
  if (index >= bufferSize) index = 0;

  return output;
}
