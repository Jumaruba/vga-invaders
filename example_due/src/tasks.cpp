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
extern volatile int alive_aliens;

void taskRight() {
    colMove += digitalRead(RIGHT_PIN) && currentCol < LAST_COL;
}

void taskLeft() {
    colMove -= digitalRead(LEFT_PIN) && currentCol > FIRST_COL ? 1 : 0;
}

void taskMiddle() {
    if (currentBulletLine == BULLET_INACTIVE_LINE && digitalRead(SHOOT_PIN) == HIGH) {
        currentBulletCol = currentCol;
        currentBulletLine = BULLET_START_LINE - BULLET_LENGTH;
    }
}

void taskDrawBullet() {
    // Verify top screen limit
    if (currentBulletLine > BULLET_LENGTH && currentBulletLine != BULLET_INACTIVE_LINE) {
        drawBullet();
        currentBulletLine-=BULLET_SPEED;
        if(checkBulletCollision()){
            deleteShoot(currentBulletLine);
            currentBulletLine = BULLET_INACTIVE_LINE;
            if(alive_aliens == 0){
                initAliens();
                alive_aliens = ALIENS_NUM;
            }
        }
    } else {
        deleteShoot(currentBulletLine); //ERRADO PRA CARALHO
        currentBulletLine = BULLET_INACTIVE_LINE;
    }
}

/**
 * @brief This function is critical since it can be interrupted many times.  
 * 
 */
void taskDrawAliens() {
    int last_alive_alien = ALIENS_NUM - 1;
    for(int i = 0 ; i < ALIENS_NUM;i++){
        if(aliens[i].isAlive)
            last_alive_alien = i;
    }        
    if (!isMoveLeft && (aliens[last_alive_alien].col + SQUARE_SIZE) >= ALIEN_MAXY) {
        isMoveLeft = true;
    } else if (aliens[0].col <= 1 && isMoveLeft) {
        isMoveLeft = false;
    }

    int move = isMoveLeft ? -1 : 1;
    int deletePosCol = isMoveLeft ? SQUARE_SIZE : 0;

    for (int alienIdx = 0; alienIdx < ALIENS_NUM; alienIdx++) {
        byte current_color = aliens[alienIdx].isAlive ? GREEN : WHITE;

        // Clean previous line
        for (int i = aliens[alienIdx].row; i < SQUARE_SIZE + aliens[alienIdx].row; i++) {
            fb[i][aliens[alienIdx].col + deletePosCol] = WHITE;
        }

        
        aliens[alienIdx].col += move;

        if(!aliens[alienIdx].isAlive)
            continue;
        // DrawAlien: Draws the alien in the next column
        for (int i = aliens[alienIdx].col; i < aliens[alienIdx].col + SQUARE_SIZE; i++) {
            for (int j = aliens[alienIdx].row; j < aliens[alienIdx].row + SQUARE_SIZE; j++) {
                fb[j][i] = current_color;
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