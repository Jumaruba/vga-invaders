#include "../include/game.h"

extern byte fb[LINES][COLS];
int squareSize = 10;
int startPos = 100;
volatile int currentPos = 0;
volatile byte incomingByte; 

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

// TODO
void moveRight()
{
    int val = digitalRead(LEFT_PIN);
    if (val == HIGH)
    {
        Serial.write("here");
        // move

    }
}

// TODO
void moveLeft()
{
}

// TODO
void shoot()
{
}

void moveWithoutInput()
{   
    printMatrix(fb); 
    for (int k = 0; k < 210; k++)
    {   
        // Clean previous line
        for (int m = startPos; m < startPos + squareSize; m++)
        {
            fb[m][startPos + k - 1] = RED;
        }
        delay(50);
        // Write new line
        for (int i = startPos + k; i < squareSize + startPos + k; i++)
        {
            for (int j = startPos; j < squareSize + startPos; j++)
            {
                fb[j][i] = BLUE;
                Serial.write(fb[i][j]);
            }
        }
    }
    printf("END OF loops"); 
}

void printMatrix(byte matrix[LINES][COLS]){
  for (int i = 0 ; i < LINES; i++){
    for (int j = 0 ; j < COLS; j++){
      Serial.write(matrix[i][j]); 
    }
    Serial.write("\n"); 
  }
}