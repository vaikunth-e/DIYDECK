#ifndef REVERB_DENORMALS_H
#define REVERB_DENORMALS_H

// forces extremely tiny values back to exactly zero to avoid denormalization
static inline float undenormalize(float x) {
  if (x > -1.0e-20f && x < 1.0e-20f) return 0.0f;
  return x;
}

#endif
