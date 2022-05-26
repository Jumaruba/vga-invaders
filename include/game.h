#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "macros.h"

using namespace std; 

/*
volatile struct square {
    byte x_pos = 0;
    byte y_pos = 0;
};
*/

void initMatrix();

void moveRight();

void moveLeft();

void shoot();

void moveWithoutInput(); 

void printMatrix(byte matrix[LINES][COLS]);

#endif