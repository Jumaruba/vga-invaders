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
  Serial.begin(9600);
  for(int i=0;i<320;i++)for(int j=0;j<240;j++)fb[j][i]=j+i;
  
  pinMode(3,OUTPUT);  pinMode(2,OUTPUT);                      // vsync=3 hsync=2
  /*pinMode(25,OUTPUT);*/ pinMode(26,OUTPUT);                     // blue  (26=msb,25=lsb)
  /*pinMode(27,OUTPUT); pinMode(28,OUTPUT);*/ pinMode(14,OUTPUT); // green (14=msb,28,27=lsb)
  /*pinMode(15,OUTPUT); pinMode(29,OUTPUT);*/ pinMode(11,OUTPUT); // red   (11=msb,29,15=lsb)

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

using namespace std;

void loop(){
  Serial.println("entry");

  for(int i=0; i<320; i++){
    byte val = 0;
    if(i < 107)   val = 0b10000000; 
    else if (i < 214) val = 0b00010000; 
    else val = 0b00000010;
 
    for (int j = 0 ; j < 240; j++){
      fb[j][i] = val; 
    } 
  }
  
  Serial.println("Before for");
  for(;;);
  Serial.println("exit");
}