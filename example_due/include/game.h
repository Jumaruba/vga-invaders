#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "macros.h"
#include "alien.h"

using namespace std; 

void initMatrix();

void initAliens();

void drawSquare();

void drawBullet();

void deleteShoot(int line);

boolean checkBulletCollision();

/**
 * TASKS:
 * - macro cycle:
 *
 * - draw aliens; time:
 * - draw square; time:
 * - draw bullet; time:
 *
 *
 * - left button
 * - right button
 * - center button
 */

#endif
