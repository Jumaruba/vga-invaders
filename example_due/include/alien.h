#ifndef ALIEN_H 
#define ALIEN_H 

#include <Arduino.h> 

struct alien {
  int row;
  int col;
  boolean isAlive = true;
};

#endif 
