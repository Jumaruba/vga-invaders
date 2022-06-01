#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

#include "alien.h"
#include "macros.h"

using namespace std;

// INIT ==================================
void initMatrix();

void initAliens();

// DRAW ==================================
void drawShip();

void drawBullet();

// CLEAN =================================
void cleanShipRight();

void cleanShipLeft();

void deleteShoot(int line);


// OTHERS ===============================
boolean checkBulletCollision();

#endif
