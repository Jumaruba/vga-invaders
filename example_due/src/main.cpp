#include <Arduino.h>

#include "../include/game.h"
#include "../include/macros.h"
#include "../include/signal.h"
#include "../include/tasks.h"

void setup() {
    initMatrix();  
    initAliens(); 
    delay(100);     // TODO search it more
    setupClock(); 
    drawSquare(); 
}

void loop() {
    taskRight();
    delay(10);
    taskLeft();
    delay(10);
    taskDrawPlayer();
    delay(10);
    taskDrawAliens();
    delay(10);
    taskMiddle();
    delay(10);
    taskDrawBullet();
    delay(10);
}