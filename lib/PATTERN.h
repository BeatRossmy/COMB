#ifndef PATTERN_h
#define PATTERN_h

char shift (char pattern, int steps) {
  // TODO: handle step values greater then 6 or less then -6
  //int sign = (abs(steps)==steps)1:-1;
  //steps = ((sign*steps)%6)*sign;

  steps = enclose(steps);

  char out = pattern;
  // positive = leftshift
  if (steps>0) out =  (out<<steps) | (out>>(6-steps));
  // negative = rightshift
  else if (steps<0) out = (out>> -steps) | (out<<(6+steps));
  return (out & 0x3F);
}

void printChar (char pattern) {
  for (int b=0; b<8; b++) Serial.print((pattern>>(7-b))&0x01);
  Serial.println();
}

#endif
