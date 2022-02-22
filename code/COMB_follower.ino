/*
   TEENSY LC * * * * * * * * * * *

   PIN 22:  touch plate 0
   PIN 23:  touch plate 1
   PIN 3:  touch plate 2
   PIN 4:  touch plate 3
   PIN 15: touch plate 4
   PIN 16: touch plate 5

   PIN 17: NeoPixel data

   PIN 18: SDA (i2c)
   PIN 19: SCL (i2c)

   PIN 5-10: send/receive

   MALE:   [+5V  SDA  S/R  SCL  GND]
   FEMALE: [GND  SCL  S/R  SDA  +5V]
*/

#include <i2c_t3.h>
#include <Adafruit_NeoPixel.h>
#include <COMB.h>

const char address = 0x77;
const int id = (address==0x66)?1:7;

IntervalTimer senderTimer;

char cmd;

void receiveEvent(size_t count) {
  cmd = Wire.readByte();
  if (cmd==EXCHANGE) {
    char p;
    if (Wire.available()) p = Wire.readByte();
    if (Wire.available()) {
      // mask eventually corrupted because of interrupted connection
      mask = Wire.readByte();
      // pattern only gets overwritten if next byte available
      pattern = (p>>6==0x03)?(p&0x3F):pattern;
    }
    change = true;
  }
  while (Wire.available()) Wire.readByte();
  
}

void requestEvent(void) {
  if (cmd == EXCHANGE) {
    Wire.write(pattern);
    Wire.write(CONFIRMATION);
  }
}

volatile int pulseCounter = 0;

void pulse() {
  for (int p=0; p<6; p++) 
    digitalWrite(p+5,(pulseCounter<id+p)?LOW:HIGH);
  pulseCounter = (pulseCounter+1)%PULSE_COUNT;
}

void setup() {
  Wire.begin(I2C_SLAVE, address, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE);
  Wire.setDefaultTimeout(TIME_OUT);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  for (int pin=5; pin<11; pin++)
    pinMode(pin,OUTPUT);

  senderTimer.begin(pulse, PULSE_LENGTH);
  
  LED.begin();
  LED.setBrightness(255);
  
  INIT_TOUCHPLATES();
}

void loop() {
  HANDLE_TOUCHPLATES(pattern,change);
  // handle LEDs
  if (change) {
    change = false;
    drawLEDs(pattern,mask);
  }
}
