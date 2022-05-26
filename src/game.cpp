#include "../include/game.h"

int squareSize = 10;
int startPos = 100;
volatile int currentPos = 0;
volatile byte incomingByte; 
byte fb[LINES][COLS];

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
    for (int k = 0; k < 210; k++)
    {
        for (int m = startPos; m < startPos + squareSize; m++)
        {
            fb[m][startPos + k - 1] = RED;
        }
        delay(50);
        for (int i = startPos + k; i < squareSize + startPos + k; i++)
        {
            for (int j = startPos; j < squareSize + startPos; j++)
            {
                fb[j][i] = BLUE;
            }
        }
    }
}