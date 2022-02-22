#ifndef PATH_h
#define PATH_h

const uint8_t MAXLENGTH = 12;

struct PATH {
  uint8_t length, intervalLength, intervalCount;
  uint8_t path [MAXLENGTH];
};

const PATH KICK = {6,6,1,{0,1,2,3,4,5}};

const PATH SNARE = {4,2,2,{2,3,8,9}};
const PATH CLAP = {4,2,2,{1,4,7,10}};
const PATH RIM = {4,2,2,{0,5,6,11}};

const PATH HH = {12,4,3,{0,1,2,3,8,9,10,11,16,17,12,13}};
const PATH OHH = {6,2,3,{5,4,7,6,15,14}};

#endif
