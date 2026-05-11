#include "encoder_control.h"

namespace {

static uint8_t clkPin_;
static uint8_t dtPin_;
static uint8_t pwm1Pin_;
static uint8_t pwm2Pin_;

static uint8_t currentIndex = 0;
static bool changedFlag = false;

static bool lastClkState = HIGH;
static uint32_t lastStepMs = 0;

static const uint32_t STEP_DELAY_MS = 150;
static const uint32_t PWM_FREQ_HZ = 20000;

static const EncoderSetting settings[] = {
  {100.0f,       338628, 2,   3},
  {137.5979878f,246099, 2,   3},
  {189.3320626f,178854, 3,   4},
  {260.5171085f,129983, 3,   4},
  {358.4662992f,94466,  3,   4},
  {493.2424149f,68653,  5,   5},
  {678.6916381f,49894,  6,   6},
  {933.8660376f,36261,  7,   7},
  {1284.980877f,26353,  9,   9},
  {1768.107831f,19152,  12,  12},
  {2432.880798f,13919,  17,  17},
  {3347.595025f,10116,  23,  24},
  {4606.223396f,7352,   33,  36},
  {6338.070708f,5343,   47,  53},
  {8721.057762f,3883,   70,  80},
  {12000.0f,     2822,   100, 100}
};

static const uint8_t NUM_SETTINGS = sizeof(settings) / sizeof(settings[0]);

static uint8_t percentToPwm8(uint8_t percent) {
  if (percent >= 100) return 255;
  return (uint8_t)((percent * 255UL) / 100UL);
}

static void applyCurrentSetting() {
  const EncoderSetting &s = settings[currentIndex];

  analogWrite(pwm1Pin_, percentToPwm8(s.pwm1Percent));
  analogWrite(pwm2Pin_, percentToPwm8(s.pwm2Percent));

  changedFlag = true;
}

static void stepForward() {
  if (currentIndex < NUM_SETTINGS - 1) {
    currentIndex++;
  } else {
    currentIndex = 0;
  }

  applyCurrentSetting();
}

static void stepBackward() {
  if (currentIndex > 0) {
    currentIndex--;
  } else {
    currentIndex = NUM_SETTINGS - 1;
  }

  applyCurrentSetting();
}

} // namespace

void encoderControlInit(
    uint8_t clkPin,
    uint8_t dtPin,
    uint8_t swPin,
    uint8_t pwm1Pin,
    uint8_t pwm2Pin
) {
  (void)swPin;

  clkPin_ = clkPin;
  dtPin_ = dtPin;
  pwm1Pin_ = pwm1Pin;
  pwm2Pin_ = pwm2Pin;

  pinMode(clkPin_, INPUT_PULLUP);
  pinMode(dtPin_, INPUT_PULLUP);

  pinMode(pwm1Pin_, OUTPUT);
  pinMode(pwm2Pin_, OUTPUT);

  // 8 bit pwm makes analogWrite values 0 to 255
  analogWriteResolution(8);
  analogWriteFrequency(PWM_FREQ_HZ);

  currentIndex = 0;
  changedFlag = false;
  lastClkState = digitalRead(clkPin_);
  lastStepMs = millis();

  applyCurrentSetting();
}

void encoderControlUpdate() {
  bool clkState = digitalRead(clkPin_);
  bool dtState = digitalRead(dtPin_);
  uint32_t now = millis();

  // only react to a falling edge on clk
  if (lastClkState == HIGH && clkState == LOW) {
    // reject extra edges from bounce
    if (now - lastStepMs >= STEP_DELAY_MS) {
      lastStepMs = now;

      if (dtState == HIGH) {
        stepForward();
      } else {
        stepBackward();
      }
    }
  }

  lastClkState = clkState;
}

bool encoderControlChanged() {
  if (changedFlag) {
    changedFlag = false;
    return true;
  }

  return false;
}

uint8_t encoderControlGetIndex() {
  return currentIndex;
}

EncoderSetting encoderControlGetSetting() {
  return settings[currentIndex];
}

void encoderControlSetIndex(uint8_t index) {
  if (index >= NUM_SETTINGS) {
    index = NUM_SETTINGS - 1;
  }

  currentIndex = index;
  applyCurrentSetting();
}