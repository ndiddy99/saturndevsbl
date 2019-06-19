#include "sprite.h"
#include "print.h"


//320x224, 8x16 font = 14 rows, 40 columns
#define ROWS 14
#define COLS 40
#define FONT_X 8
#define FONT_Y 16
Uint16 text[ROWS][COLS];


void print_init() {
	int i, j;
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			text[i][j] = 65535;
		}
	}
}

static int num_digits(Uint32 num) {
	if (num < 10)
		return 1;
	else if (num < 100)
		return 2;
	else if (num < 1000)
		return 3;
	else if (num < 10000)
		return 4;
	else if (num < 100000)
		return 5;
	else if (num < 1000000)
		return 6;
	else if (num < 10000000)
		return 7;
	else if (num < 100000000)
		return 8;
	else if (num < 1000000000)
		return 9;
	else
		return 10;
}

void print_num(Uint32 num, int row, int col) {
	int right_col = col + 9; //rightmost column
	int i;
	for (i = 0; i < 9; i++) {
		text[row][right_col--] = num % 10;
		num /= 10;
	}
}

void print_display() {
	int i, j;
	SPRITE_INFO text_spr;
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			if (text[i][j] != 65535) {
				make_sprite(text[i][j],
					MTH_IntToFixed(j << 3), // * FONT_X
					MTH_IntToFixed(i << 4), // * FONT_Y
					&text_spr);
				draw_sprite(&text_spr);
			}
		}
	}
}
