#ifndef HELPER_h
#define HELPER_h

int enclose (int value) {
  int out = abs(value)%6;
  return out*((value!=abs(value))?-1:1);
}

int closestInteger (float input) {
  int output = input;
  if (input-output>=0.5) output++;
  return output;
}

int countConnectedSides (int sides[6]) {
  int count = 0;
  for (int n=0; n<6; n++) {
    if (sides[n]>0) count++;
  }
  return count;
}

int getFirst (int sides[6]) {
  if (sides[0]>0 && sides[5]>0) return sides[5];
  for (int n=0; n<6; n++)
    if (sides[n]>0) return sides[n];
  return -1;
}

int getSecond (int sides[6]) {
  if (sides[0]>0 && sides[5]>0) return sides[0];
  bool first = false;
  for (int n=0; n<6; n++) {
    if (sides[n]>0 && !first) first = true;
    else if (sides[n]>0 && first) return sides[n];
  }
  return -1;
}

int getIndexOfFirst (int sides[6]) {
  if (sides[0]>0 && sides[5]>0) return 5;
  for (int n=0; n<6; n++)
    if (sides[n]>0) return n;
  return -1;
}

int getIndexOfSecond (int sides[6]) {
  if (sides[0]>0 && sides[5]>0) return 0;
  bool first = false;
  for (int n=0; n<6; n++) {
    if (sides[n]>0 && !first) first = true;
    else if (sides[n]>0 && first) return n;
  }
  return -1;
}

bool twoAdjacentSides (int sides[6]) {
  for (int n=0; n<6; n++) {
    if (sides[n]>0 && (sides[(n+1)%6]>0 || sides[(n+5)%6]>0)) return true;
  }
  return false;
}

char sideToAddress (int side) {
  return (side/6.0<=1)?0x66:0x77;
}

int delimitSide (int side) {
  // 1-6 -> 1-6 , 7-12 -> 1-6 , 13-18 -> 1-6 , ...
  return ((side-1)%6)+1;
}

#endif
