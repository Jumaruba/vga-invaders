#include "../include/game.h"

#include <Arduino.h>

int startCol = COLS / 2 - SQUARE_SIZE / 2;

extern byte fb[LINES][COLS];  // Code matrix
alien aliens[ALIENS_NUM];
volatile int currentCol = startCol;
volatile int currentBulletLine = BULLET_INACTIVE_LINE;
volatile int currentBulletCol;
volatile int alive_aliens = ALIENS_NUM;

void initMatrix() {
    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 240; j++) {
            fb[j][i] = WHITE;
        }
    }
}



void initAliens() {
    // For even number of aliens.
    /*
    //mult = i == ALIENS_PER_LINE ? 0 : mult;
    aliens[i].row = ALIEN_MINX;
    aliens[i].col = ALIEN_MINY + i * SQUARE_SIZE_DOUBLE;
    */
    for (int i = 0, mult = 0; i < ALIENS_NUM; i++, mult++) {
        aliens[i].row = ALIEN_MINX;
        aliens[i].col = ALIEN_MINY + i * SQUARE_SIZE_DOUBLE;
        aliens[i].isAlive = true;
    }
    /*
    for (int i = 0, mult = 0; i < ALIENS_NUM; i++, mult++) {
        mult = i == ALIENS_PER_LINE ? 0 : mult;
        aliens[i].row = ALIEN_MINX + (i != 0 ? (SQUARE_SIZE_DOUBLE * mult) : 0);
        aliens[i].col = ALIEN_MINY + (i >= ALIENS_PER_LINE ? SQUARE_SIZE_DOUBLE : 0);
    }
    */
}

// DRAWS ==================================================

void drawShip() {
    for (int row = SHIP_START_ROW; row < SHIP_END_ROW; row++) {
        for (int col = currentCol; col < currentCol + SQUARE_SIZE; col++) {
            fb[row][col] = BLUE;
        }
    }
}

void drawBullet() {
    for (int i = currentBulletLine; i > currentBulletLine - BULLET_LENGTH; i--) {
        fb[i][currentBulletCol + SQUARE_SIZE_HALF] = RED;
    }
    if(currentBulletLine != BULLET_INACTIVE_LINE)
        for (int i = 1 ; i < 1+ BULLET_SPEED; i++) {
            fb[currentBulletLine + i][currentBulletCol + SQUARE_SIZE_HALF] = WHITE;        
        }

}

// CLEANS =============================================================

void cleanShipRight() {
    for (int row = SHIP_START_ROW; row < SHIP_END_ROW; row++) {
        fb[row][currentCol + SQUARE_SIZE] = WHITE;
    }
}

void cleanShipLeft() {
    for (int row = SHIP_START_ROW; row < SHIP_END_ROW; row++) {
        fb[row][currentCol] = WHITE;
    }
}

void cleanAlien(int row, int col) {
    for (int i = col; i < col + SQUARE_SIZE; i++) {
        for (int j = row; j < row + SQUARE_SIZE; j++) {
            fb[j][i] = WHITE;
        }
    }
}

void deleteShoot(int line) {
    for (int i = line + BULLET_SPEED; i >= line - BULLET_LENGTH; i--) {
        fb[i][currentBulletCol + SQUARE_SIZE_HALF] = WHITE;
    }
}


// GAME LOGIC FUNCTION

boolean checkBulletCollision() {
    for (int alienIdx = 0; alienIdx < ALIENS_NUM; alienIdx++) {
        alien temp_alien = aliens[alienIdx];
        if (!temp_alien.isAlive)
            continue;
        for (int temp_bulletLine = currentBulletLine; temp_bulletLine > currentBulletLine - BULLET_LENGTH; temp_bulletLine--) {
            // Checks if bullet is inside the aliens bounds
            if (temp_bulletLine >= temp_alien.row 
                && temp_bulletLine <= (temp_alien.row + SQUARE_SIZE) 
                && currentBulletCol + SQUARE_SIZE_HALF >= temp_alien.col
                && currentBulletCol + SQUARE_SIZE_HALF <= (temp_alien.col + SQUARE_SIZE)) {
                aliens[alienIdx].isAlive = false;
                alive_aliens--;
                cleanAlien(temp_alien.row, temp_alien.col);
                return true;
            }
        }
    }
    return false;
}
