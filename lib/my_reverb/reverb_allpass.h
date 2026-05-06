#ifndef REVERB_ALLPASS_H
#define REVERB_ALLPASS_H

class ReverbAllpass {
public:
  void setBuffer(float *bufferPtr, int size);
  void setFeedback(float value);
  void mute();
  float process(float input);

private:
  float *buffer = nullptr;
  int bufferSize = 0;
  int index = 0;
  float feedback = 0.5f;
};

#endif
