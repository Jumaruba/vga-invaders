#include <Arduino.h>

#include "../include/game.h"
#include "../include/macros.h"
#include "../include/signal.h"
#include "../include/tasks.h"

void setup() {
    Serial.begin(9600);
    initMatrix();
    initAliens();
    delay(100);
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
    delayMicroseconds(695); 
}

void cycle3(){
    taskLeft();
    taskDrawShip(); 
    taskMiddle();
    taskDrawBullet();
    delayMicroseconds(687); 
}

void cycle4(){
    taskRight();
    taskDrawShip();
    delayMicroseconds(695);
}

void loop() {
    cycle1();     
    cycle2();      
    cycle3();     
    cycle4();   
}