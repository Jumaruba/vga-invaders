/* Arduino Due VGA-Out v0.2 by stimmer
http://arduino.cc/forum/index.php/topic,130742.0.html */

inline void digitalWriteDirect(int pin, boolean val){
  if(val) g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
}

volatile short line;
byte fb[240][320];

#define do20(x) x x x x x x x x x x x x x x x x x x x x
#define do80(x)  do20(x) do20(x) do20(x) do20(x) 
#define do320(x) do80(x) do80(x) do80(x) do80(x) 
#define MNOP(x) asm volatile (" .rept " #x "\n\t nop \n\t .endr \n\t")

void TC0_Handler()
{
    long dummy=REG_TC0_SR0; 
                           
    if(line < 480){        
        byte * p=fb[line>>1];
        MNOP(160);
        do320(REG_PIOD_ODSR = *p++;MNOP(2);)
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

  REG_PIOD_OWER= 0xff;
  REG_PMC_PCER0= 1<<27;  
  REG_PIOB_PDR = 1<<25; 
  REG_PIOB_ABSR= 1<<25; 
  REG_TC0_WPMR = 0x54494D00; 
  REG_TC0_CMR0 = 0b00000000000010011100010000000000;
//  REG_TC0_CMR0 = 0b00000000000001101100010000000000; // this inverts hsync
  REG_TC0_RC0  = 1334; 
  REG_TC0_RA0  = 1174;  
  REG_TC0_CCR0 = 0b101;    
  REG_TC0_IER0 = 0b00010000; 
  REG_TC0_IDR0 = 0b11101111; 
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