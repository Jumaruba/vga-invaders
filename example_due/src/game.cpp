#include "../include/game.h"

#include <Arduino.h>

int startCol = COLS / 2 - SQUARE_SIZE / 2;

extern byte fb[LINES][COLS];    // Code matrix 
volatile alien aliens[ALIENS_NUM];
volatile int currentCol = startCol;
volatile int currentBulletLine = BULLET_INACTIVE_LINE;
volatile int currentBulletCol;

void initMatrix() {
    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 240; j++) {
            fb[j][i] = WHITE;
        }
    }
}

void initAliens() {
    // For even number of aliens. 
    for (int i = 0, mult = 0; i < ALIENS_NUM; i++, mult++) {
        mult = i == ALIENS_PER_LINE ? 0: mult; 
        aliens[i].row = ALIEN_MINX + (i != 0 ? (SQUARE_SIZE_DOUBLE * mult) : 0);
        aliens[i].col = ALIEN_MINY + (i >= ALIENS_PER_LINE ? SQUARE_SIZE_DOUBLE : 0);
    }
}

void drawSquare() {
    for (int row = SHIP_START_ROW; row < SHIP_END_ROW; row++) {
        for (int col = currentCol; col < currentCol + SQUARE_SIZE; col++) {
            fb[row][col] = BLUE;
        }
    }
} 

void drawBullet() {
    for (int i = currentBulletLine; i > currentBulletLine - BULLET_LENGTH; i--) {
        fb[i][currentBulletCol + SQUARE_SIZE / 2] = RED;
    }
    fb[currentBulletLine + 1][currentBulletCol + SQUARE_SIZE / 2] = WHITE;
}

void deleteShoot(int line) {
    for (int i = line + 1; i >= line - BULLET_LENGTH; i--) {
        fb[i][currentBulletCol] = WHITE;
    }
}

boolean checkBulletCollision() {
    for (int alienIdx = 0; alienIdx < ALIENS_NUM; alienIdx++) {
        if (!aliens[alienIdx].isAlive)
            continue;
        for (int temp_bulletLine = currentBulletLine; temp_bulletLine > currentBulletLine - BULLET_LENGTH; temp_bulletLine--) {
            // Checks if bullet is inside the aliens bounds
            if (temp_bulletLine >= aliens[alienIdx].row && temp_bulletLine <= (aliens[alienIdx].row + SQUARE_SIZE) && currentBulletCol >= aliens[alienIdx].col && currentBulletCol <= (aliens[alienIdx].col + SQUARE_SIZE)) {
                aliens[alienIdx].isAlive = false;
                return true;
            }
        }
    }
    return false;
}
