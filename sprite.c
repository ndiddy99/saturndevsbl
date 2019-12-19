#include <sega_def.h>
#include <sega_mth.h>
#include <machine.h>
#define _SPR2_
#include <sega_spr.h>
#include <sega_scl.h>
#include <string.h>

#include "cd.h"
#include "collision.h"
#include "graphicrefs.h"
#include "print.h"
#include "scroll.h"
#include "sprite.h"
#include "vblank.h"
#include "guy.c"
#include "test.c"

int num_sprites = 0;
SPRITE_INFO sprites[SPRITE_LIST_SIZE];
//normalize diagonal speed
#define DIAGONAL_MULTIPLIER (MTH_FIXED(0.8))

#define CommandMax    300
#define GourTblMax    300
#define LookupTblMax  100
#define CharMax       128 //CHANGE WHEN YOU INCREASE TILES BEYOND THIS POINT
#define DrawPrtyMax   256
SPR_2DefineWork(work2D, CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)

char image_buf[2048];


void sprite_init() {
	int count, i;

	SCL_Vdp2Init();
	SPR_2Initial(&work2D);
	count = 0;
	SCL_SetColRamMode(SCL_CRM24_1024);
	
	SetVblank(); //setup vblank routine
	set_imask(0);
	
	SPR_2FrameChgIntr(1); //wait until next frame to set color mode
	SCL_DisplayFrame();
	
	cd_open("TEST.SPR", image_buf, 768);
	// for (i = 0; i < 64 * 2; i += 2) {
	// 	SPR_2SetChar((Uint16)count, COLOR_5, 0, dimensions[i], dimensions[i + 1], (char *)tiles[count]);
	// 	count++;
	// }
	// memset(&image_buf, 2, 768);

	// print_num(test, 5, 5);
	SPR_2SetChar(0, COLOR_0, 0, guy_width, guy_height, image_buf);
	SCL_AllocColRam(SCL_SPR, 16, OFF);
	SCL_SetColRam(SCL_SPR, 0, 16, &test_pal);
	for (i = 0; i < SPRITE_LIST_SIZE; i++) {
		sprites[i].xSize = NODISP;
	}
	SCL_DisplayFrame();
}

void sprite_draw(SPRITE_INFO *info) {
	XyInt xy[4];
	Fixed32 xOffset, yOffset, sin, cos, scaledX, scaledY;
	int i;
	
	if (info->scale == MTH_FIXED(1) && info->angle == 0) {
		xy[0].x = (Sint16)MTH_FixedToInt(info->xPos);
		xy[0].y = (Sint16)MTH_FixedToInt(info->yPos);
		SPR_2NormSpr(0, info->mirror, COLOR_0, 0, info->char_num, xy, NO_GOUR); //4bpp normal sprite
	}
	
	else if (info->angle == 0){
		xy[0].x = (Sint16)MTH_FixedToInt(info->xPos);
		xy[0].y = (Sint16)MTH_FixedToInt(info->yPos);
		//the way scale works is by giving the x/y coordinates of the top left and
		//bottom right corner of the sprite
		xy[1].x = (Sint16)(MTH_FixedToInt(MTH_Mul(info->xSize, info->scale) + info->xPos));
		xy[1].y = (Sint16)(MTH_FixedToInt(MTH_Mul(info->ySize, info->scale) + info->yPos));
		SPR_2ScaleSpr(0, info->mirror, COLOR_0, 0, info->char_num, xy, NO_GOUR); //4bpp scaled sprite
	}
	
	else {
		//offset of top left sprite corner from the origin
		xOffset = -(MTH_Mul(info->xSize >> 1, info->scale));
		yOffset = -(MTH_Mul(info->ySize >> 1, info->scale));
		sin = MTH_Sin(info->angle);
		cos = MTH_Cos(info->angle);
		scaledX = info->xPos + MTH_Mul(info->xSize >> 1, info->scale);
		scaledY = info->yPos + MTH_Mul(info->ySize >> 1, info->scale);
		//formula from
		//https://gamedev.stackexchange.com/questions/86755/
		for (i = 0; i < 4; i++) {
			if (i == 1) xOffset = -xOffset; //upper right
			if (i == 2) yOffset = -yOffset; //lower right
			if (i == 3) xOffset = -xOffset; //lower left
			xy[i].x = (Sint16)MTH_FixedToInt(MTH_Mul(xOffset, cos) - 
				MTH_Mul(yOffset, sin) + scaledX);
			xy[i].y = (Sint16)MTH_FixedToInt(MTH_Mul(xOffset, sin) +
				MTH_Mul(yOffset, cos) + scaledY);
		}
		SPR_2DistSpr(0, info->mirror, COLOR_0, 0, info->char_num, xy, NO_GOUR); //4bpp distorted sprite
	}
}

void sprite_make(int tile_num, Fixed32 x, Fixed32 y, SPRITE_INFO *ptr) {
	ptr->char_num = tile_num;
	ptr->xPos = x;
	ptr->yPos = y;
	ptr->mirror = 0;
	ptr->xSize = MTH_IntToFixed(dimensions[tile_num << 1]);
	ptr->ySize = MTH_IntToFixed(dimensions[(tile_num << 1) + 1]);
	ptr->dx = 0;
	ptr->dy = 0;
	ptr->speed = 0;
	ptr->scale = MTH_FIXED(1);
	ptr->angle = 0;
	ptr->animTimer = 0;
	ptr->animCursor = 0;
	ptr->state = SPRITE_NULL;
	ptr->facing = SPRITE_DOWN;
	ptr->iterate = NULL;
}

void sprite_draw_all() {
	int i;
	SPRITE_INFO tmp;
	for (i = 0; i < SPRITE_LIST_SIZE; i++) {
		if (sprites[i].xSize != NODISP && sprites[i].iterate != NULL) {
			sprites[i].iterate(&sprites[i]);
		}
	}
	for (i = 0; i < SPRITE_LIST_SIZE; i++) {
		if (sprites[i].xSize != NODISP) {
			memcpy((void *)&tmp, (void *)&sprites[i], sizeof(SPRITE_INFO));
			tmp.xPos -=scrolls_x[0];
			tmp.yPos -=scrolls_y[0];
			sprite_draw(&tmp);
		}
	}
}

SPRITE_INFO *sprite_next() {
	int i;
	for (i = 0; i < SPRITE_LIST_SIZE; i++) {
		if (sprites[i].xSize == NODISP) {
			num_sprites++;
			sprites[i].index = i;
			sprites[i].state = SPRITE_NULL;
			sprites[i].iterate = NULL;
			return &sprites[i];
		}
	}
	return NULL;
}

void sprite_delete(SPRITE_INFO *sprite) {
	sprite->xSize = NODISP;
	sprite->iterate = NULL;
	num_sprites--;
}

void sprite_deleteall() {
	int i;
	for (i = 0; i < SPRITE_LIST_SIZE; i++) {
		sprites[i].xSize = NODISP;
	}
	num_sprites = 0;
}

Uint16 sprite_move(SPRITE_INFO *sprite, int collision) {
    switch(sprite->state) {
        case SPRITE_UP:
            sprite->yPos -= sprite->speed;
			if (collision) {
				collision_detect_up(sprite, 1);
			}
        break;
        case SPRITE_DOWN:
            sprite->yPos += sprite->speed;
			if (collision) {
				collision_detect_down(sprite, 1);
			}
        break;
        case SPRITE_LEFT:
            sprite->xPos -= sprite->speed;
			if (collision) {
				collision_detect_left(sprite, 1);
			}
        break;
        case SPRITE_RIGHT:
            sprite->xPos += sprite->speed;
			if (collision) {
				collision_detect_right(sprite, 1);
			}
        break;
        case SPRITE_UPLEFT:
            sprite->xPos -= MTH_Mul(sprite->speed, DIAGONAL_MULTIPLIER);
            sprite->yPos -= MTH_Mul(sprite->speed, DIAGONAL_MULTIPLIER);
			if (collision) {
				collision_detect_up_left(sprite);
			}
        break;
        case SPRITE_UPRIGHT:
            sprite->xPos += MTH_Mul(sprite->speed, DIAGONAL_MULTIPLIER);
            sprite->yPos -= MTH_Mul(sprite->speed, DIAGONAL_MULTIPLIER);
			if (collision) {
				collision_detect_up_right(sprite);
			}
        break;
        case SPRITE_DOWNLEFT:
            sprite->xPos -= MTH_Mul(sprite->speed, DIAGONAL_MULTIPLIER);
            sprite->yPos += MTH_Mul(sprite->speed, DIAGONAL_MULTIPLIER);
			if (collision) {
				collision_detect_down_left(sprite);
			}
        break;
        case SPRITE_DOWNRIGHT:
            sprite->xPos += MTH_Mul(sprite->speed, DIAGONAL_MULTIPLIER);
            sprite->yPos += MTH_Mul(sprite->speed, DIAGONAL_MULTIPLIER);
			if (collision) {
				collision_detect_down_right(sprite);
			}
        break;
    }
	return collision_get_tile(sprite->xPos, sprite->yPos);
}
