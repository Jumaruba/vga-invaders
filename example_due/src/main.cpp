#include <Arduino.h>
#include "../include/macros.h"

inline void digitalWriteDirect(int pin, boolean val);

void drawAliens();
void initAliens();
void moveAliens();
void initMatrix();
void drawSquare();
void moveRight();
void moveLeft();
void shoot();
inline void deleteShoot(int line);

volatile short line;
volatile byte current_color = BLUE;
volatile int currentBulletLine = BULLET_INACTIVE_LINE;
volatile int currentBulletCol;

byte fb[LINES][COLS];
struct alien
{
  byte row;
  byte col;
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
boolean isMoveLeft = false; 

void setup()
{
  Serial.begin(9600);
  initMatrix();
  initAliens();
  drawSquare();

  pinMode(VSYNC_PIN, OUTPUT); // vsync=3
  pinMode(HSYNC_PIN, OUTPUT); // hsync=2
  pinMode(RED_PIN, OUTPUT);   // Red
  pinMode(BLUE_PIN, OUTPUT);  // Blue
  pinMode(GREEN_PIN, OUTPUT); // Green
  pinMode(LEFT_PIN, INPUT);   // Left
  pinMode(SHOOT_PIN, INPUT);  // Middle
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

  drawAliens();

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

void drawSquare()
{
  for (int row = SQUARE_LINE; row < SQUARE_LINE + SQUARE_SIZE; row++)
  {
    for (int col = currentCol; col < currentCol + SQUARE_SIZE; col++)
    {
      fb[row][col] = BLUE;
    }
  }
}

void initAliens()
{
  int baseRow = 1;
  int baseCol = 50;
  // for even ALIENS_NUM
  for (int i = 0, mult = 0; i < ALIENS_NUM; i++, mult++)
  {
    if (i == ALIENS_NUM / 2)
      mult = 0;
    aliens[i].row = baseRow + (i > 0 ? (SQUARE_SIZE_DOUBLE * mult) : 0);
    aliens[i].col = baseCol + (i >= ALIENS_NUM / 2 ? SQUARE_SIZE_DOUBLE : 0);
  }
}

void drawAliens()
{
  if (!isMoveLeft && aliens[ALIENS_NUM -1].col >= ALIEN_MAXY){
    isMoveLeft = true;
    Serial.write("MOVE LEFT");
  } else if (aliens[0].col <= 1 && isMoveLeft) {
    isMoveLeft = false; 
    Serial.write("MOVE RIGHT");
  }

  int move = isMoveLeft ? -1 : 1;
  int deletePosCol = isMoveLeft ? SQUARE_SIZE : 0;

  for (int alienIdx = 0; alienIdx < ALIENS_NUM; alienIdx++)
  { 
    // Clean previous line
    for (int i = aliens[alienIdx].row; i < SQUARE_SIZE + aliens[alienIdx].row; i++)
    {
      fb[i][aliens[alienIdx].col + deletePosCol] = WHITE;
    }

    aliens[alienIdx].col += move;

    // Draw next column
    for (int i = aliens[alienIdx].col; i < aliens[alienIdx].col + SQUARE_SIZE; i++)
    {
      for (int j = aliens[alienIdx].row; j < aliens[alienIdx].row + SQUARE_SIZE; j++)
      {
        fb[j][i] = GREEN;
      }
    }
  }
}

void drawBullet()
{
  for (int i = currentBulletLine; i > currentBulletLine - BULLET_LENGTH; i--)
  {
    fb[i][currentBulletCol] = RED;
  }
  fb[currentBulletLine + 1][currentBulletCol] = WHITE;
}

void moveLeft()
{
  if (currentCol <= FIRST_COL)
  {
    return;
  }

  // Clean the first column.
  for (int i = SQUARE_LINE; i < SQUARE_SIZE + SQUARE_LINE; i++)
  {
    fb[i][currentCol + SQUARE_SIZE] = WHITE;
  }

  currentCol--;

  // Print the square.
  drawSquare();
}

void moveRight()
{
  if (currentCol >= LAST_COL)
  {
    return;
  }

  // Clean the first column.
  for (int i = SQUARE_LINE; i < SQUARE_SIZE + SQUARE_LINE; i++)
  {
    fb[i][currentCol] = WHITE;
  }

  currentCol++;

  // Print the square.
  for (int row = SQUARE_LINE; row < SQUARE_LINE + SQUARE_SIZE; row++)
  {
    for (int col = currentCol; col < currentCol + SQUARE_SIZE; col++)
    {
      fb[row][col] = BLUE;
    }
  }
}

void shoot()
{
  if (currentBulletLine == BULLET_INACTIVE_LINE && digitalRead(SHOOT_PIN) == HIGH)
  {
    currentBulletCol = currentCol;
    currentBulletLine = BULLET_START_LINE;
  }

  // Verify top screen limit
  if (currentBulletLine > BULLET_LENGTH)
  {
    drawBullet();
    currentBulletLine--;
  }
  else
  {
    deleteShoot(currentBulletLine);
    currentBulletLine = BULLET_INACTIVE_LINE;
  }
}

inline void deleteShoot(int line)
{
  for (int i = line + 1; i >= line - BULLET_LENGTH; i--)
  {
    fb[i][currentBulletCol] = WHITE;
  }
}

inline void digitalWriteDirect(int pin, boolean val)
{
  if (val)
    g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
  else
    g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
}