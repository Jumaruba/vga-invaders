#include "../include/signal.h"

volatile short line;
byte fb[LINES][COLS];

void TC0_Handler() {

    if (line < 480)
    {
        byte *p = fb[line >> 1];
        MNOP(160);
        do320(REG_PIOD_ODSR = *p++; MNOP(2);)
            REG_PIOD_ODSR = 0;
    }
    if (line == 490)
        digitalWriteDirect(VSYNC_PIN, 1);
    if (line == 492)
        digitalWriteDirect(VSYNC_PIN, 0);

    line++;
    if (line == 525)
        line = 0;
}

void setupClock() {

    pinMode(VSYNC_PIN, OUTPUT); // vsync=3
    pinMode(HSYNC_PIN, OUTPUT); // hsync=2
    pinMode(RED_PIN, OUTPUT);   // Red
    pinMode(BLUE_PIN, OUTPUT);  // Blue
    pinMode(GREEN_PIN, OUTPUT); // Green
    pinMode(LEFT_PIN, INPUT);   // Left
    pinMode(MIDDLE_PIN, INPUT); // Middle
    pinMode(RIGHT_PIN, INPUT);  // Right

    REG_PIOD_OWER = 0xff;
    REG_PMC_PCER0 = 1 << 27;
    REG_PIOB_PDR = 1 << 25;
    REG_PIOB_ABSR = 1 << 25;
    REG_TC0_WPMR = 0x54494D00;
    REG_TC0_CMR0 = 0b00000000000010011100010000000000;

    REG_TC0_RC0 = 1334;
    REG_TC0_RA0 = 1174;
    REG_TC0_CCR0 = 0b101;
    REG_TC0_IER0 = 0b00010000;
    REG_TC0_IDR0 = 0b11101111;
    NVIC_EnableIRQ(TC0_IRQn);
}

inline void digitalWriteDirect(int pin, boolean val) {
    if (val)
        g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
    else
        g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
}