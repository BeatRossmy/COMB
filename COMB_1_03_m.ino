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

   MASTER: 3.3V over 4.7k Ohm to SDA
           3.3V over 4.7k Ohm to SCL
*/

#include <i2c_t3.h>
#include <Adafruit_NeoPixel.h>
#include <COMB.h>

int step = -1;

SEQUENCE s0,s1,s2;
TRIGGER base,clap,snare,rim,hh,ohh;

int nbrIDs [6];
CONNECTION mstr,slv1,slv2;

MODES mode = DRUM;
MODES lastMode = NONE;
bool modeChange = false;

void getNbrs () {
  for (int n=0; n<6; n++)
    nbrIDs[n] = closestInteger((float)pulseIn(5+n,LOW,PULSE_LENGTH*PULSE_COUNT*3)/(float)PULSE_LENGTH);
}

void detectMode (MODES& mode) {
  int connectedSides = countConnectedSides(nbrIDs);
  
  if (connectedSides == 1) {
    mode = STICK;

    mstr.address = 0x00;
    mstr.rotation = getIndexOfFirst(nbrIDs)-4;
    
    slv1.address = sideToAddress(getFirst(nbrIDs));
    slv1.rotation = delimitSide(getFirst(nbrIDs))-2;
    
    slv2.address = 0x00;
    slv2.rotation = 0;
  }
  else if (connectedSides == 2 && twoAdjacentSides(nbrIDs)) {
    mode = TRIANGLE;

    mstr.address = 0x00;
    mstr.rotation = getIndexOfFirst(nbrIDs)-4;
    
    slv1.address = sideToAddress(getFirst(nbrIDs));
    slv1.rotation = delimitSide(getFirst(nbrIDs))-2;
    
    slv2.address = sideToAddress(getSecond(nbrIDs));
    slv2.rotation = delimitSide(getSecond(nbrIDs))-3;
  }
  else {
    mode = DRUM;

    mstr.address = 0x00;
    mstr.rotation = 0;
    
    slv1.address = 0x00;
    slv1.rotation = 0;
    
    slv2.address = 0x00;
    slv2.rotation = 0;
  }

  if (lastMode!=mode) modeChange = true;
  lastMode = mode;
}

byte counter; 
byte CLOCK = 248; 
byte START = 250; 
byte CONTINUE = 251; 
byte STOP = 252;

void RealTimeSystem(byte realtimebyte) {
  if (realtimebyte == CLOCK) {
    counter = (counter+1)%6;
    
    if (counter>1 && counter<=5) {
      getNbrs();
      detectMode(mode);
    }
    
    else if (counter == 0) {
      base.release();
      clap.release();
      snare.release();
      rim.release();
      hh.release();
      ohh.release();
    }
    
    else if (counter == 1) {
      step = (step+1) % 12;
      
      base.trigger(s0.getStep(step,KICK));
      snare.trigger(s1.getStep(step,SNARE));
      clap.trigger(s1.getStep(step,CLAP));
      rim.trigger(s1.getStep(step,RIM));
      hh.trigger(s2.getStep(step,HH));
      ohh.trigger(s2.getStep(step,OHH));
      
      mask = EMPTY;
      
      if (mode==STICK) {
        char p = shift(s1.getPart(1),slv1.rotation) | ((modeChange)?0xC0:0x00);
        s1.setPart(1,shift(exchangeData(slv1.address,p,calcMask(mode,1,step,slv1.rotation)),-slv1.rotation));
      }
      else if (mode==TRIANGLE) {
        char p = shift(s2.getPart(1),slv1.rotation) | ((modeChange)?0xC0:0x00);
        s2.setPart(1,shift(exchangeData(slv1.address,p,calcMask(mode,1,step,slv1.rotation)),-slv1.rotation));
        
        delay(5);

        p = shift(s2.getPart(2),slv2.rotation) | ((modeChange)?0xC0:0x00);
        s2.setPart(2,shift(exchangeData(slv2.address,p,calcMask(mode,2,step,slv2.rotation)),-slv2.rotation));
      }
      else if (mode==DRUM) {
        
        exchangeData(0x66,0xC0,EMPTY);
        exchangeData(0x77,0xC0,EMPTY);
      }
      
      change = true;
      modeChange = false;
    }
    
  }

  if (realtimebyte == START || realtimebyte == CONTINUE || realtimebyte == STOP) {
    counter = 0;
    step = -1;
    change = true;

    base.release();
    clap.release();
    snare.release();
    rim.release();
    hh.release();
    ohh.release();
  }
}

void setup() {
  for (int pin=5; pin<11; pin++)
    pinMode(pin, INPUT_PULLUP);

  INIT_TOUCHPLATES();

  LED.begin();
  LED.setBrightness(255);

  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE);
  Wire.setDefaultTimeout(TIME_OUT);

  usbMIDI.setHandleRealTimeSystem(RealTimeSystem);

  base.init(36);
  clap.init(39);
  snare.init(38);
  rim.init(37);
  hh.init(42);
  ohh.init(46);
}

void loop() {
  usbMIDI.read();
  
  // set pattern to part of current sequence
  if (mode==DRUM) pattern = s0.getPart(0);
  else if (mode==STICK) pattern = shift(s1.getPart(0),mstr.rotation);
  else if (mode==TRIANGLE) pattern = shift(s2.getPart(0),mstr.rotation);
  
  // handle touch input perfomed on current pattern
  HANDLE_TOUCHPLATES(pattern,change);
  
  // insert pattern into current sequence
  if (mode==DRUM) s0.setPart(0,pattern);
  else if (mode==STICK) s1.setPart(0,shift(pattern,-mstr.rotation));
  else if (mode==TRIANGLE) s2.setPart(0,shift(pattern,-mstr.rotation));

  // handle LEDs
  if (change) {
    change = false;
    drawLEDs(pattern,calcMask(mode,0,step,mstr.rotation));
  }
}
