#include <sega_def.h>
#include <sega_mth.h>
#include <machine.h>
#define _SPR2_
#include <sega_spr.h>
#include <sega_scl.h>
#include <string.h>

#include "collision.h"
#include "graphicrefs.h"
#include "print.h"
#include "scroll.h"
#include "sprite.h"
#include "vblank.h"

#define SPRITE_LIST_SIZE 200
int num_sprites = 0;
SPRITE_INFO sprites[SPRITE_LIST_SIZE];
//normalize diagonal speed
#define DIAGONAL_MULTIPLIER (MTH_FIXED(0.8))

#define CommandMax    300
#define GourTblMax    300
#define LookupTblMax  100
#define CharMax       32 //CHANGE WHEN YOU INCREASE TILES BEYOND THIS POINT
#define DrawPrtyMax   256
SPR_2DefineWork(work2D, CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)

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
	for (i = 0; i < 21 * 2; i += 2) {
		SPR_2SetChar((Uint16)count, COLOR_5, 0, dimensions[i], dimensions[i + 1], (char *)tiles[count]);
		count++;
	}
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
		SPR_2NormSpr(0, info->mirror, COLOR_5, 0, info->char_num, xy, NO_GOUR); //rgb normal sprite
	}
	
	else if (info->angle == 0){
		xy[0].x = (Sint16)MTH_FixedToInt(info->xPos);
		xy[0].y = (Sint16)MTH_FixedToInt(info->yPos);
		//the way scale works is by giving the x/y coordinates of the top left and
		//bottom right corner of the sprite
		xy[1].x = (Sint16)(MTH_FixedToInt(MTH_Mul(MTH_IntToFixed(info->xSize), info->scale) + info->xPos));
		xy[1].y = (Sint16)(MTH_FixedToInt(MTH_Mul(MTH_IntToFixed(info->ySize), info->scale) + info->yPos));
		SPR_2ScaleSpr(0, info->mirror, COLOR_5, 0, info->char_num, xy, NO_GOUR); //rgb scaled sprite
	}
	
	else {
		//offset of top left sprite corner from the origin
		xOffset = -(MTH_Mul(MTH_IntToFixed(info->xSize >> 1), info->scale));
		yOffset = -(MTH_Mul(MTH_IntToFixed(info->ySize >> 1), info->scale));
		sin = MTH_Sin(info->angle);
		cos = MTH_Cos(info->angle);
		scaledX = info->xPos + MTH_Mul(MTH_IntToFixed(info->xSize >> 1), info->scale);
		scaledY = info->yPos + MTH_Mul(MTH_IntToFixed(info->ySize >> 1), info->scale);
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
		SPR_2DistSpr(0, info->mirror, COLOR_5, 0, info->char_num, xy, NO_GOUR); //rgb distorted sprite
	}
}

void sprite_make(int tile_num, Fixed32 x, Fixed32 y, SPRITE_INFO *ptr) {
	ptr->char_num = tile_num;
	ptr->index = 0; //updated by the sprite_next function
	ptr->xPos = x;
	ptr->yPos = y;
	ptr->mirror = 0;
	ptr->xSize = MTH_IntToFixed(dimensions[tile_num << 1]);
	ptr->ySize = MTH_IntToFixed(dimensions[(tile_num << 1) + 1]);
	ptr->speed = 0;
	ptr->scale = MTH_FIXED(1);
	ptr->angle = 0;
	ptr->animTimer = 0;
	ptr->state = SPRITE_NULL;
	ptr->facing = SPRITE_DOWN;
	ptr->iterate = NULL;
}

void sprite_draw_all() {
	int i;
	SPRITE_INFO tmp;
	for (i = 0; i < SPRITE_LIST_SIZE; i++) {
		if (sprites[i].xSize != NODISP && sprites[i].iterate != NULL) {
			print_num(sprites[i].state, 5, 0);
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
	print_num(num_sprites, 4, 0);
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
