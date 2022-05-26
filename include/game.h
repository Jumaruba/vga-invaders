#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "macros.h"
#include "common.h"

using namespace std; 

void initMatrix();

void moveRight();

void moveLeft();

void shoot();

void moveWithoutInput(); 

#endif