#include "reverb_comb.h"
#include "reverb_denormals.h"

void ReverbComb::setBuffer(float *bufferPtr, int size) {
  buffer = bufferPtr;
  bufferSize = size;
  index = 0;
  filterStore = 0.0f;
  mute();
}

void ReverbComb::setFeedback(float value) {
  feedback = value;
}

void ReverbComb::setDamping(float value) {
  damp1 = value;
  damp2 = 1.0f - value;
}

void ReverbComb::mute() {
  if (!buffer) return;

  for (int i = 0; i < bufferSize; i++) {
    buffer[i] = 0.0f;
  }

  index = 0;
  filterStore = 0.0f;
}

float ReverbComb::process(float input) {
  float output = undenormalize(buffer[index]);

  // Low-pass damping inside the feedback loop. More damping removes more high
  // frequencies from the reverb tail.
  filterStore = undenormalize((output * damp2) + (filterStore * damp1));

  buffer[index] = input + (filterStore * feedback);

  index++;
  if (index >= bufferSize) index = 0;

  return output;
}
