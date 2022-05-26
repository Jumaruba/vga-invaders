#include <Arduino.h>

using namespace std;

#define LINES 240
#define COLS 320
#define FIRST_COL 0
#define LAST_COL 320

#define RED 0b10000000
#define GREEN 0b00010000
#define BLUE 0b00000010
#define BLACK 0b00000000

#define VSYNC_PIN 3
#define HSYNC_PIN 2
#define RED_PIN 11
#define GREEN_PIN 14
#define BLUE_PIN 26
#define LEFT_PIN 54
#define MIDDLE_PIN 55
#define RIGHT_PIN 56

#define SQUARE_SIZE = 10;

#define do20(x) x x x x x x x x x x x x x x x x x x x x
#define do80(x) do20(x) do20(x) do20(x) do20(x)
#define do320(x) do80(x) do80(x) do80(x) do80(x)
#define MNOP(x) asm volatile(" .rept " #x "\n\t nop \n\t .endr \n\t")

inline int getNumNeighbors(int line, int col);
inline byte getCellColor(int line, int col);
inline void digitalWriteDirect(int pin, boolean val);
void initMatrix();
void initSquare();
void moveRight();
void moveLeft();


volatile short line;
volatile byte current_color = BLUE;
byte fb[LINES][COLS];

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

void setup()
{
  Serial.begin(9600);
  initMatrix();
  initSquare();

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

int squareLine = 200;
int squareSize = 10;
int startCol = COLS / 2 - squareSize / 2;
volatile byte incomingByte;
volatile int currentCol = startCol;

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

  /*
  for (int k = 0; k < 210; k++){
    for (int m = startPos; m < startPos + squareSize; m++){
      fb[m][startPos+k-1] = RED;
    }
    delay(50);
    for (int i = startPos + k; i < squareSize + startPos +k; i++){
        for (int j = startPos; j < squareSize + startPos; j++){
            fb[j][i] = BLUE;
        }
    }
  }
  */
}

void initMatrix()
{
  for (int i = 0; i < 320; i++)
  {
    for (int j = 0; j < 240; j++)
    {
      fb[j][i] = RED;
    }
  }
}

void initSquare()
{
  for (int i = squareLine; i < squareLine + squareSize; i++) {
    for (int j = startCol; j < startCol + squareSize; j++) {
      fb[i][j] = BLUE;
    }
  }
}

void drawSquare() {
  
}

// TODO
void moveLeft() {
  Serial.write("Left");

  if(currentCol <= FIRST_COL){
    return;
  } 

  // Clean the first column.
  for (int i = squareLine; i < squareSize + squareLine; i++) {
    fb[i][currentCol + squareSize] = RED;
  }

  currentCol--;

  // Print the square.
  for (int row = squareLine; row < squareLine + squareSize; row++) {
    for (int col = currentCol; col < currentCol + squareSize; col++) {
      fb[row][col] = BLUE;
    }
  }
}

// TODO
void moveRight()
{
  Serial.write("right");
/*
  if(currentPos <= FIRST_COL ){
    return;
  } 

  // Clean the first column.
  for (int m = squareLine; m < squareLine + squareSize; m++) {
    fb[m][currentPos] = RED;
  }

  currentPos--;
  // Print the square.
  for (int j = currentPos; j < squareSize; j++) {
    fb[squareLine][j] = BLUE;
  }*/ 

}

// TODO
void shoot()
{
}

inline void digitalWriteDirect(int pin, boolean val)
{
  if (val)
    g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
  else
    g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
}