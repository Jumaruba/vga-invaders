#include <Arduino.h> 

/* Arduino Due VGA-Out v0.2 by stimmer
http://arduino.cc/forum/index.php/topic,130742.0.html */


/**
 * @brief Writes a value to a pin in arduino. https://forum.arduino.cc/t/port-manipulation/487729
 * 
 * @param pin The pin number.
 * @param val The value that we want to write. 
 */
inline void digitalWriteDirect(int pin, boolean val){
  if(val) g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
}

volatile short line;
byte fb[240][320]; // 320x240

#define do20(x) x x x x x x x x x x x x x x x x x x x x
#define do80(x)  do20(x) do20(x) do20(x) do20(x) 
#define do320(x) do80(x) do80(x) do80(x) do80(x) 
#define MNOP(x) asm volatile (" .rept " #x "\n\t nop \n\t .endr \n\t") // A strange way to compute times. 

void TC0_Handler()
{
    long dummy=REG_TC0_SR0;   
    
    // 640x480 
    if(line < 480){        
        byte * p=fb[line>>1];                 // Dividing the number by two, the lines 2 and 3, for example, will correspond to the line 1. 
        MNOP(160);                            // Taking the time to do the backporch. One instruction takes 1/84Mhz, thus 160/(84*10^6) = 1.9*10^(-6)
        do320(REG_PIOD_ODSR = *p++; MNOP(2);) // For each pixel
        REG_PIOD_ODSR = 0;
    }
    
    if(line==490) digitalWriteDirect(3,1); //or digitalWriteDirect(3,0); to invert vsync
    if(line==492) digitalWriteDirect(3,0); //or digitalWriteDirect(3,1); to invert vsync
    
    line++; if(line == 525) line=0;
}

void setup(){
  for(int i=0;i<320;i++)for(int j=0;j<240;j++)fb[j][i]=j+i;
  
  pinMode(3,OUTPUT);  pinMode(2,OUTPUT);                      // vsync=3 hsync=2
  pinMode(25,OUTPUT); pinMode(26,OUTPUT);                     // blue  (26=msb,25=lsb)
  pinMode(27,OUTPUT); pinMode(28,OUTPUT); pinMode(14,OUTPUT); // green (14=msb,28,27=lsb)
  pinMode(15,OUTPUT); pinMode(29,OUTPUT); pinMode(11,OUTPUT); // red   (11=msb,29,15=lsb)

  REG_PIOD_OWER= 0xff;                                    // Enable writing in the PIO_ODSR
  REG_PMC_PCER0= 1<<27;                                   // Enables the peripheral clock in pin 27 (datasheet page 542). To disable uses the REG_PMC_PCDR0 register. 
  REG_PIOB_PDR = 1<<25;                                   // Disables the PIO from controlling the correspond PIN 25. 
  REG_PIOB_ABSR= 1<<25;                                   // This is the AB select register. Assigns the I/O line to the Peripheral B function. 
  REG_TC0_WPMR = 0x54494D00;                              // Enable write to register. 
  REG_TC0_CMR0 = 0b00000000000010011100010000000000;
  //  REG_TC0_CMR0 = 0b00000000000001101100010000000000;  // This inverts hsync
  REG_TC0_RC0  = 1334;                                    // Counter period
  REG_TC0_RA0  = 1174;                                    // PWM value  
  REG_TC0_CCR0 = 0b101;                                   // 1[SWTRG]- the counter is reset and the clock is restarted, 0[CLKDIS]- no effect (CLKDIS); 1[CLKEN]-enables the clock if CLKDIS is not 1.
  REG_TC0_IER0 = 0b00010000;                              // Interrupt enable register - enables the RC compare interrupt. 
  REG_TC0_IDR0 = 0b11101111;                              // Disables interrupts excepts calendar.
  NVIC_EnableIRQ(TC0_IRQn);
}

#include <complex>
using namespace std;

const byte cmap[]={0b00000000,0b11100000,0b11100100,0b11101000,0b11101100,0b11110000,0b11110100,0b11111000,0b11111100,
                   0b11011100,0b10111100,0b10011100,0b01111100,0b01011100,0b00111100,0b00011100,0b00011101,0b00011110,
                   0b00011111,0b00011011,0b00010111,0b00010011,0b00001111,0b00001011,0b00000111,0b00000011,0b00100011,
                   0b01000011,0b01100011,0b10000011,0b10100011,0b11000011,0b11100011,0b11100010,0b11100001,0b11100000,0b00000000};
void loop(){
  for(int i=0;i<320;i++){
    for(int j=0;j<240;j++){     
      complex<float> z(0,0),c((i+180.0)/1280.0,(j+640.0)/1280.0);
      int n;
      for(n=1;n<sizeof(cmap);n++){
        z=z*z+c;
        if(norm(z)>4.0)break;
      }
      fb[j][i]=cmap[sizeof(cmap)-n];
    }
  }
  for(;;);
}