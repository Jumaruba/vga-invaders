#ifndef MACROS_H 
#define MACROS_H 

#define RED   0b10000000
#define GREEN 0b00010000
#define BLUE  0b00000010
#define WHITE 0b11111111

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
#define SQUARE_SIZE   25
#define LAST_COL      320 - SQUARE_SIZE
#define DELAY         30 
#define BULLET_LENGTH 6
#define BULLET_WIDTH 2
#define BULLET_SPEED BULLET_LENGTH
#define ALIENS_NUM 4
#define ALIENS_PER_LINE ALIENS_NUM
#define ALIEN_MINX 1
#define ALIEN_MINY 0
#define ALIEN_MAXY 320
#define SQUARE_SIZE_HALF SQUARE_SIZE/2
#define SQUARE_SIZE_DOUBLE SQUARE_SIZE*2
#define BULLET_INACTIVE_LINE 0
#define BULLET_START_LINE SHIP_START_ROW - 2


// SQUARE =================================================================== 

#define SHIP_START_ROW   200                            // The row where the ship (main square) is located. 
#define SHIP_END_ROW SQUARE_SIZE + SHIP_START_ROW       // The position of the ship last row.      



#define do20(x) x x x x x x x x x x x x x x x x x x x x
#define do80(x) do20(x) do20(x) do20(x) do20(x)
#define do320(x) do80(x) do80(x) do80(x) do80(x)
#define MNOP(x) asm volatile(" .rept " #x "\n\t nop \n\t .endr \n\t")

#endif 
