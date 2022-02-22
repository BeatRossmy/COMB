#ifndef COMB_h
#define COMB_h

#include "Arduino.h"
#include "HELPER.h"
#include "PATH.h"
#include "TOUCH_PLATE.h"
#include "COMMUNICATION.h"
#include "PATTERN.h"
#include "SEQUENCE.h"
#include "TRIGGER.h"
#include "LED.h"

enum MODES {
  NONE = -1,
  DRUM = 0,
  STICK = 1,
  TRIANGLE = 2
};

#define I2C_RATE     100000

#define PULSE_LENGTH 10  // lower values will cause the interrupt code to crash
#define PULSE_COUNT  20

struct CONNECTION {
    char address = 0x00;
    int rotation = 0;
    void set (CONNECTION c) {
      address = c.address;
      rotation = c.rotation;
    }
};

bool change;
char mask, pattern;
TOUCH_PLATE plates [6];

void INIT_TOUCHPLATES () {
  plates[0].init(22, 0);
  plates[1].init(23, 1);
  plates[2].init(3, 2);
  plates[3].init(4, 3);
  plates[4].init(15, 4);
  plates[5].init(16, 5);
}

void HANDLE_TOUCHPLATES (char& pattern, bool& change) {
  for (int p=0; p<6; p++) plates[p].handle(pattern,change);
}

char getPoint (int index, int step, const PATH path) {
  char point = EMPTY;
  step %= path.length;
  if (step/path.intervalLength==index)
    point = DOT<<(path.path[step]%6);
  return point&0x3F;
}

char calcMask (MODES mode, int index, int step, int rotation) {
  char mask = 0x00;
  if (step<0) step = 0;
  if  (mode==DRUM)
    mask = getPoint(index,step,KICK);
  else if (mode==STICK)
    mask = getPoint(index,step,SNARE)|getPoint(index,step,CLAP)|getPoint(index,step,RIM);
  else if (mode==TRIANGLE)
    mask = getPoint(index,step,HH)|getPoint(index,step,OHH);
  return shift(mask,rotation);
}

#endif
