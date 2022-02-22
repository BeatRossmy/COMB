#ifndef COMMUNICATION_h
#define COMMUNICATION_h

// Command definitions
#define STEP         0x10
#define READ         0x30
#define EXCHANGE     0x40
#define GET_NEIGHBOURS 0x50

#define CONFIRMATION 0xFF

#define TIME_OUT 100000

// MASTER ======================================================================

void resetI2C () {
  if (Wire.status()==I2C_TIMEOUT || Wire.status()==I2C_ADDR_NAK || Wire.status()==I2C_ARB_LOST) {
    Wire.resetBus();
  }
}

char exchangeData (char address, char pattern, char mask) {
  char output = 0x00;
  bool confirmation = false;

  Wire.beginTransmission(address);

  Wire.write(EXCHANGE);
  Wire.write(pattern);
  Wire.write(mask);

  Wire.endTransmission(I2C_NOSTOP);

  Wire.requestFrom(address, 2, I2C_STOP);
  if (Wire.available()) output = Wire.readByte();
  if (Wire.available()) {
    Wire.readByte();
    confirmation = true;
  }
  //while (Wire.available()) Wire.readByte();
  resetI2C();
  // if confirmation received return output, else return last pattern
  return (confirmation)?output:pattern;
}

void debugI2C () {

    // I2C DEBUG
    /*
    if (Wire.status()==I2C_WAITING) {
      LED.setPixelColor(0,magenta);
      LED.show();
    }

    else if (Wire.status()==I2C_SENDING) {
      LED.setPixelColor(1,magenta);
      LED.show();
    }

    else if (Wire.status()==I2C_SEND_ADDR) {
      LED.setPixelColor(2,magenta);
      LED.show();
    }

    else if (Wire.status()==I2C_RECEIVING) {
      LED.setPixelColor(3,magenta);
      LED.show();
    }

    else if (Wire.status()==I2C_TIMEOUT) { // !!!!!!!!!
      LED.setPixelColor(4,magenta);
      LED.show();
    }

    else if (Wire.status()==I2C_ADDR_NAK) { // !!!!!!!!!
      LED.setPixelColor(5,magenta);
      LED.show();
    }

    else if (Wire.status()==I2C_DATA_NAK) {
      LED.setPixelColor(0,yellow);
      LED.show();
    }

    else if (Wire.status()==I2C_ARB_LOST) { // !!!!!!!!!
      LED.setPixelColor(1,yellow);
      LED.show();
    }

    else if (Wire.status()==I2C_SLAVE_TX) {
      LED.setPixelColor(2,yellow);
      LED.show();
    }

    else if (Wire.status()==I2C_SLAVE_RX) {
      LED.setPixelColor(3,yellow);
      LED.show();
    }
    */
}

#endif
