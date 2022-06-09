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
    taskDrawAliens(); 
    taskLeft(); 
    taskDrawShip(); 
}

void cycle2(){
    taskRight();
    taskDrawShip(); 
    delay(3);
}

void cycle3(){
    taskMiddle();
    taskDrawBullet();
    delay(3);
}

void cycle4(){
    taskLeft();
    taskDrawShip();
    delay(3);
}



void loop() {
    cycle1();
    cycle2();
    cycle4(); 
    cycle3();
    cycle2();
    delayMicroseconds(3060);
 
}