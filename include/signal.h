#include <Arduino.h>
#include "macros.h"
#include "variables.h"


volatile short line;

void TC0_Handler(); 

void setupClock(); 

inline void digitalWriteDirect(int pin, boolean val); 

