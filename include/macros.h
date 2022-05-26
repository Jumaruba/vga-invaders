#ifndef MACROS_H 
#define MACROS_H 

#define LINES 240
#define COLS 320

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

#define SQUARE_SIZE = 10


#define do20(x) x x x x x x x x x x x x x x x x x x x x
#define do80(x) do20(x) do20(x) do20(x) do20(x)
#define do320(x) do80(x) do80(x) do80(x) do80(x)
#define MNOP(x) asm volatile(" .rept " #x "\n\t nop \n\t .endr \n\t")

#endif 