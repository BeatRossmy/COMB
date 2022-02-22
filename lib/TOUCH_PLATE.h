#ifndef TOUCH_PLATE_h
#define TOUCH_PLATE_h

const float uTHR = 1.035;
const float lTHR = 1.02;
const float percentage = 0.05;
const float initCycles = 50.0;

void toggleStep (char& pattern, int index) {pattern ^= (0x01<<index);}

bool getStep (char pattern, int index) {return pattern&(0x01<<index);}

class TOUCH_PLATE {
  private:
  uint8_t ID,PIN;
  bool state;
  float average, value;

  public:
  void init (int pin, int id) {
    PIN = pin;
    ID = id;
    for (int n = 0; n < initCycles; n++) average += touchRead(PIN);
    average /= initCycles;
  }

  void handle (char& pattern, bool& change) {
    value = touchRead(PIN);
    if (value > uTHR * average && !state) {
      if (!state) toggleStep(pattern,ID);
      state = true;
      change = true;
    }
    else if (value < lTHR * average && state) {
      state = false;
      change = true;
    }
    if (!state) average = percentage * value + (1.0 - percentage) * average;
  }

  float getAverage () {return average;}

  float getValue () {return value;}
};
#endif
