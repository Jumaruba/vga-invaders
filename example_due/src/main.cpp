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

void loop() {
    taskRight();  
    delay(20); 
    taskLeft();  
    delay(20); 
    taskDrawShip(); 
    delay(20); 
    taskDrawAliens();  
    delay(20);   
    taskMiddle();  
    delay(20);
    //int t1 = micros(); 
    taskDrawBullet();
    //int t2 = micros(); 
    //Serial.print(t2-t1);
    //Serial.print(" ");
    delay(20);
}