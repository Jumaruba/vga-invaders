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
    colMove += digitalRead(RIGHT_PIN) && currentCol < LAST_COL;
}

void taskLeft() {
    colMove -= digitalRead(LEFT_PIN) && currentCol > FIRST_COL ? 1 : 0;
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

void taskDrawShip() {
    int newCol = currentCol + colMove;
    if (newCol != currentCol) {
        newCol > currentCol ? cleanShipLeft() : cleanShipRight();  // Clean the column.
        currentCol = newCol;                                       // Update the position.
        drawShip();
        colMove = 0;
    }
}