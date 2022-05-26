#ifndef SIGNAL_H
#define SIGNAL_H

#include <Arduino.h>
#include "macros.h"
#include "common.h"

using namespace std;

void TC0_Handler(); 

void setupClock(); 

inline void digitalWriteDirect(int pin, boolean val); 

#endif 