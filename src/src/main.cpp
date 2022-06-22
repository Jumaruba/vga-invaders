#include <Arduino.h>

#include "../include/game.h"
#include "../include/macros.h"
#include "../include/signal.h"
#include "../include/tasks.h"

void setup() {
    Serial.begin(9600);
    initMatrix();
    initAliens();
    delay(100);  // TODO search it more
    setupClock();
    drawShip();
}

void cycle1(){
    taskLeft(); 
    taskDrawShip(); 
    taskDrawAliens(); 
}

void cycle2(){
    taskRight();
    taskDrawShip(); 
    delay(3);
}

void cycle3(){
    taskLeft();
    taskDrawShip(); 
    taskMiddle();
    taskDrawBullet();
    delayMicroseconds(2940);
}

void cycle4(){
    taskRight();
    taskDrawShip();
    delay(3);
}



void loop() {
    cycle1();
    cycle2();
    cycle3();
    cycle4(); 
    delayMicroseconds(3060);
 
}