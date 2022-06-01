#include "../include/tasks.h"
#include "../include/alien.h"
#include "../include/game.h"

extern volatile int currentCol;
extern byte fb[LINES][COLS];   
volatile int colMove = 0;
volatile boolean isMoveLeft = false;
extern volatile int currentBulletLine; 
extern volatile int currentBulletCol;
extern volatile alien aliens[ALIENS_NUM]; 

void taskRight() {
    if (digitalRead(RIGHT_PIN) && currentCol < LAST_COL) {
        colMove++;
    }
}

void taskLeft() {
    if (digitalRead(LEFT_PIN) && currentCol > FIRST_COL) {
        colMove--;
    }
}

void taskMiddle() {
    if (currentBulletLine == BULLET_INACTIVE_LINE && digitalRead(SHOOT_PIN) == HIGH) {
        currentBulletCol = currentCol;
        currentBulletLine = BULLET_START_LINE;
    }
}

void taskDrawBullet() {
    // Verify top screen limit
    if (currentBulletLine > BULLET_LENGTH) {
        drawBullet();
        currentBulletLine--;
    } else {
        deleteShoot(currentBulletLine);
        currentBulletLine = BULLET_INACTIVE_LINE;
    }
}

void taskDrawAliens() {
    if (!isMoveLeft && aliens[ALIENS_NUM - 1].col >= ALIEN_MAXY) {
        isMoveLeft = true;
    } else if (aliens[0].col <= 1 && isMoveLeft) {
        isMoveLeft = false;
    }

    int move = isMoveLeft ? -1 : 1;
    int deletePosCol = isMoveLeft ? SQUARE_SIZE : 0;

    for (int alienIdx = 0; alienIdx < ALIENS_NUM; alienIdx++) {
        if (!aliens[alienIdx].isAlive)
            continue;

        // Clean previous line
        for (int i = aliens[alienIdx].row; i < SQUARE_SIZE + aliens[alienIdx].row; i++) {
            fb[i][aliens[alienIdx].col + deletePosCol] = WHITE;
        }

        aliens[alienIdx].col += move;

        // Draw next column
        for (int i = aliens[alienIdx].col; i < aliens[alienIdx].col + SQUARE_SIZE; i++) {
            for (int j = aliens[alienIdx].row; j < aliens[alienIdx].row + SQUARE_SIZE; j++) {
                fb[j][i] = GREEN;
            }
        }
    }
}

void taskDrawPlayer() {
    int newCol = currentCol + colMove;

    if (newCol != currentCol) {
        // Clean previous columns
        if (newCol > currentCol) {
            for (int col = currentCol; col < newCol; col++) {
                for (int row = SHIP_START_ROW; row < SQUARE_SIZE + SHIP_START_ROW; row++) {
                    fb[row][col] = WHITE;
                }
            }
        } else {
            for (int col = currentCol; col >= newCol; col--) {
                for (int row = SHIP_START_ROW; row < SQUARE_SIZE + SHIP_START_ROW; row++) {
                    fb[row][col + SQUARE_SIZE] = WHITE;
                }
            }
        }

        currentCol = newCol;

        // Print the square.
        for (int row = SHIP_START_ROW; row < SHIP_START_ROW + SQUARE_SIZE; row++) {
            for (int col = currentCol; col < currentCol + SQUARE_SIZE; col++) {
                fb[row][col] = BLUE;
            }
        }
        colMove = 0;
    }
}