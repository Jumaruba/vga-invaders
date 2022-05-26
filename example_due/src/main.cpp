#include <Arduino.h>

using namespace std;

#define RED   0b10000000
#define GREEN 0b00010000
#define BLUE  0b00000010
#define WHITE 0b11111111
#define BLACK 0b00000000

#define VSYNC_PIN  3
#define HSYNC_PIN  2
#define RED_PIN    11
#define GREEN_PIN  14
#define BLUE_PIN   26
#define LEFT_PIN   54
#define SHOOT_PIN  55
#define RIGHT_PIN  56

#define LINES         240
#define COLS          320
#define FIRST_COL     0
#define SQUARE_SIZE   10
#define LAST_COL      320 - SQUARE_SIZE
#define SQUARE_LINE   200
#define DELAY         30 
#define BULLET_LENGTH 6
#define ALIENS_NUM 1

#define do20(x) x x x x x x x x x x x x x x x x x x x x
#define do80(x) do20(x) do20(x) do20(x) do20(x)
#define do320(x) do80(x) do80(x) do80(x) do80(x)
#define MNOP(x) asm volatile(" .rept " #x "\n\t nop \n\t .endr \n\t")

inline int getNumNeighbors(int line, int col);
inline byte getCellColor(int line, int col);
inline void digitalWriteDirect(int pin, boolean val);

void drawAlien(int row, int col);
void initMatrix();
void drawSquare();
void moveRight();
void moveLeft();
void shoot();

volatile short line;
volatile byte current_color = BLUE;
volatile int currentBulletLine = 0;  
volatile int currentBulletCol; 


byte fb[LINES][COLS];
struct alien {
  byte x;
  byte y;
};

volatile alien aliens[ALIENS_NUM];

void TC0_Handler()
{
  long dummy = REG_TC0_SR0;

  if (line < 480)
  {
    byte *p = fb[line >> 1];
    MNOP(160);
    do320(REG_PIOD_ODSR = *p++; MNOP(2);)
        REG_PIOD_ODSR = 0;
  }
  if (line == 490)
    digitalWriteDirect(3, 1); // or digitalWriteDirect(3,0); to invert vsync
  if (line == 492)
    digitalWriteDirect(3, 0); // or digitalWriteDirect(3,1); to invert vsync

  line++;
  if (line == 525)
    line = 0;
}

int startCol = COLS / 2 - SQUARE_SIZE / 2;
volatile byte incomingByte;
volatile int currentCol = startCol;

void setup()
{
  Serial.begin(9600);
  initMatrix();
  drawSquare();
  drawAlien(SQUARE_LINE -  100 ,startCol);

  pinMode(VSYNC_PIN, OUTPUT); // vsync=3
  pinMode(HSYNC_PIN, OUTPUT); // hsync=2
  pinMode(RED_PIN, OUTPUT);   // Red
  pinMode(BLUE_PIN, OUTPUT);  // Blue
  pinMode(GREEN_PIN, OUTPUT); // Green
  pinMode(LEFT_PIN, INPUT);   // Left
  pinMode(SHOOT_PIN, INPUT); // Middle
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



void loop()
{
  if (digitalRead(RIGHT_PIN) == HIGH)
  {
    moveRight();
  }
  else if (digitalRead(LEFT_PIN) == HIGH)
  {
    moveLeft();
  }

  shoot();
  delay(DELAY);
}

void initMatrix()
{
  for (int i = 0; i < 320; i++)
  {
    for (int j = 0; j < 240; j++)
    {
      fb[j][i] = WHITE;
    }
  }
}

void drawSquare() {
  for (int row = SQUARE_LINE; row < SQUARE_LINE + SQUARE_SIZE; row++) {
    for (int col = currentCol; col < currentCol + SQUARE_SIZE; col++) {
      fb[row][col] = BLUE;
    }
  }
}

void drawAlien(int row, int col) {
  for (int i=row; i < row + SQUARE_SIZE; i++) {
    for (int j=col; j < col + SQUARE_SIZE; j++) {
      fb[i][j] = GREEN;
    }
  }
}

/*
void moveAliens() {
  for(int i=0; i < something; i++) {
    moveAlien(i);
  }
}
*/

void drawBullet(){
  for (int i = currentBulletLine; i > currentBulletLine - BULLET_LENGTH; i--){
    fb[i][currentBulletCol] = GREEN;   
  }
  fb[currentBulletLine+1][currentBulletCol] = WHITE; 
}

void moveLeft() {
  if(currentCol <= FIRST_COL){
    return;
  } 

  // Clean the first column.
  for (int i = SQUARE_LINE; i < SQUARE_SIZE + SQUARE_LINE; i++) {
    fb[i][currentCol + SQUARE_SIZE] = WHITE;
  }

  currentCol--;

  // Print the square.
  drawSquare();
}

void moveRight()
{
  if(currentCol >= LAST_COL){
    return;
  } 

  // Clean the first column.
  for (int i = SQUARE_LINE; i < SQUARE_SIZE + SQUARE_LINE; i++) {
    fb[i][currentCol] = WHITE;
  }

  currentCol++;

  // Print the square.
  for (int row = SQUARE_LINE; row < SQUARE_LINE + SQUARE_SIZE; row++) {
    for (int col = currentCol; col < currentCol + SQUARE_SIZE; col++) {
      fb[row][col] = BLUE;
    }
  }
}

void shoot(){
  // TODO: handle multiple shoots
  if (digitalRead(SHOOT_PIN) == HIGH){
    currentBulletCol = currentCol; 
    currentBulletLine = SQUARE_LINE - 1; 
  } 

  // Verify top screen limit
  if (currentBulletLine > BULLET_LENGTH) {
    drawBullet(); 
    currentBulletLine--;
  } else {
      // TODO: apagar tiro
  }
   
}

inline void digitalWriteDirect(int pin, boolean val)
{
  if (val)
    g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
  else
    g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
}