#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "macros.h"

using namespace std; 

void initMatrix();

void initAliens();

void moveLeft();

void moveRight();

void shoot();

void drawAliens();

void drawBullet();

void drawSquare();

inline void deleteShoot(int line);

#endif
