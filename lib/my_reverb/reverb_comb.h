#ifndef REVERB_COMB_H
#define REVERB_COMB_H

class ReverbComb {
public:
  void setBuffer(float *bufferPtr, int size);
  void setFeedback(float value);
  void setDamping(float value);
  void mute();
  float process(float input);

private:
  float *buffer = nullptr;
  int bufferSize = 0;
  int index = 0;
  float feedback = 0.84f;
  float damp1 = 0.20f;
  float damp2 = 0.80f;
  float filterStore = 0.0f;
};

#endif
