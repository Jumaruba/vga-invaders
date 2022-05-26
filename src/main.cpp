#include <Arduino.h>
#include "../include/macros.h"
#include "../include/signal.h"
#include "../include/game.h"

using namespace std;

void setup(){
  Serial.begin(9600);
  initMatrix();
  setupClock(); 
}

void loop()
{
  moveWithoutInput();
}
