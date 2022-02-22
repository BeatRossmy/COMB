#ifndef LED_h
#define LED_h

Adafruit_NeoPixel LED = Adafruit_NeoPixel(6, 17, NEO_GRB + NEO_KHZ800);

const int32_t red = LED.Color(255,0,0);
const int32_t green = LED.Color(0,255,0);
const int32_t blue = LED.Color(0,0,255);

const int32_t yellow = LED.Color(255,255,0);
const int32_t cyan = LED.Color(0,255,255);
const int32_t magenta = LED.Color(255,0,255);

const int32_t white = LED.Color(255,255,255);

// MASKS
const char EMPTY = 0x00;
const char DOT = 0x01;    // 000001
const char LINE = 0x07;   // 000111

void drawLEDs (char pattern, char mask) {
  for (int led=0; led<6; led++) {
    int32_t color = 0;
    if (pattern&mask&(0x01<<led)) color = cyan;
    else if (pattern&(0x01<<led)) color = blue;
    else if (mask&(0x01<<led)) color = green;
    LED.setPixelColor(led,color);
  }
  LED.show();
}

#endif
