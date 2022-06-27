#include "../include/signal.h"

volatile short line;
byte fb[LINES][COLS];

void TC0_Handler()
{

  long dummy = REG_TC0_SR0;

  // Draw current line
  if (line < 480)
  {
    byte *p = fb[line >> 1];
    MNOP(160); // Horizontal Back Porch
    do320(REG_PIOD_ODSR = *p++; MNOP(2);) 
    REG_PIOD_ODSR = 0;
  }
  // After Vertical front porch
  if (line == 490)
    digitalWriteDirect(VSYNC_PIN, 1); 
  // After Vertical Sync Pulse
  if (line == 492)
    digitalWriteDirect(VSYNC_PIN, 0);

  line++;
  // Last line, after Vertical Back Porch - reset line counter
  if (line == 525)
    line = 0; 

}

void setupClock() {
  pinMode(VSYNC_PIN, OUTPUT); // Vsync
  pinMode(HSYNC_PIN, OUTPUT); // Hsync
  pinMode(RED_PIN, OUTPUT);   // Red
  pinMode(BLUE_PIN, OUTPUT);  // Blue
  pinMode(GREEN_PIN, OUTPUT); // Green
  pinMode(LEFT_PIN, INPUT);   // Left
  pinMode(SHOOT_PIN, INPUT);  // Middle
  pinMode(RIGHT_PIN, INPUT);  // Right

  REG_PIOD_OWER = 0xff;       // Enables writing to the pins with register ODSR
  REG_PMC_PCER0 = 1 << 27;    // Enable Timer 0 (peripheral with id 27 = TC0)
  REG_PIOB_PDR = 1 << 25;     // Disable GPIO, enable peripheral (Hsync)
  REG_PIOB_ABSR = 1 << 25;    // Select peripheral B.25 (Hsync)
  REG_TC0_WPMR = 0x54494D00;  // Enable write to registers
  REG_TC0_CMR0 = 0b00000000000010011100010000000000; // Set channel mode register
                                                     // TIMER_CLOCK1 => MCK/2
                                                     // Wave form mode
                                                     // Set TIOA0 (HSYNC)

  REG_TC0_RC0 = 1334; // Counter Period = MCK/2/RC0 = 0,0315Hz => T=31.74 (HSYNC)
  REG_TC0_RA0 = 1174; // Duty Cycle = RC0 - 160 (HSYNC)
  REG_TC0_CCR0 = 0b101; // Start counter
  REG_TC0_IER0 = 0b00010000;  // Enable interrupt on counter
  REG_TC0_IDR0 = 0b11101111;  // Disable other iterrupts
  NVIC_EnableIRQ(TC0_IRQn);   // Enable TC0 interrupts
}

inline void digitalWriteDirect(int pin, boolean val) {
    if (val)
        g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
    else
        g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
}