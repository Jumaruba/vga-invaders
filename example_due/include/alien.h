#ifndef ALIEN_H 
#define ALIEN_H 

#include <Arduino.h> 

struct alien {
  int row;
  int col;
  boolean isAlive = true;
  alien(){
    row = 0;
    col = 0;
    isAlive =  true;
  }

  alien(alien volatile const& other){
    row = other.row;
    col = other.col;
    isAlive = other.isAlive;
  }
};

#endif 
