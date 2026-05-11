#ifndef ENCODER_CONTROL_H
#define ENCODER_CONTROL_H

#include <Arduino.h>

struct EncoderSetting {
  float cutoffHz;
  uint32_t resistanceOhms;
  uint8_t pwm1Percent;
  uint8_t pwm2Percent;
};

// initializer in setup
void encoderControlInit(
    uint8_t clkPin,
    uint8_t dtPin,
    uint8_t swPin,
    uint8_t pwm1Pin,
    uint8_t pwm2Pin
);

// loop()
void encoderControlUpdate();

// true once after encoder changes
bool encoderControlChanged();

// current setting index, 0 to 15
uint8_t encoderControlGetIndex();

// current setting data
EncoderSetting encoderControlGetSetting();

// force a setting manually.
void encoderControlSetIndex(uint8_t index);

#endif