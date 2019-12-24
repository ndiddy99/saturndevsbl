#include <sega_def.h>
#include <sega_mth.h>
#include <machine.h>
#define _SPR2_
#include <sega_spr.h>
#include <sega_scl.h>
#include <string.h>

#include "cd.h"
#include "graphicrefs.h"
#include "print.h"
#include "scroll.h"
#include "sprite.h"
#include "vblank.h"

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

Uint32 image_buf[1024];


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
	
	cd_load(font_name, image_buf, font_size * font_num);
	for (i = 0; i < font_num; i++) {
		SPR_2SetChar(i, COLOR_0, 0, font_width, font_height, (Uint8 *)(image_buf) + (i * font_size));
	}

	cd_load(guy_name, image_buf, guy_size * guy_num);
	for (i = 0; i < guy_num; i++) {
		SPR_2SetChar(i + font_num, COLOR_0, 16, guy_width, guy_height, (Uint8 *)(image_buf) + (i * guy_size));
	}
	// SPR_2SetChar(0, COLOR_0, 0, guy_width, guy_height, (Uint8 *)(image_buf) + 256);
	SCL_AllocColRam(SCL_SPR, 16, OFF);
	SCL_SetColRam(SCL_SPR, 0, 16, &font_pal);
	SCL_SetColRam(SCL_SPR, 16, 16, &guy_pal);
	sprite_deleteall();
	SCL_DisplayFrame();
}

void sprite_draw(SPRITE_INFO *info) {
	XyInt xy[4];
	Fixed32 xOffset, yOffset, sin, cos, scaledX, scaledY;
	int i;
	
	if (info->scale == MTH_FIXED(1) && info->angle == 0) {
		xy[0].x = (Sint16)MTH_FixedToInt(info->xPos);
		xy[0].y = (Sint16)MTH_FixedToInt(info->yPos);
		SPR_2NormSpr(0, info->mirror, COLOR_0, 0xffff, info->char_num, xy, NO_GOUR); //4bpp normal sprite
	}
	
	else if (info->angle == 0){
		xy[0].x = (Sint16)MTH_FixedToInt(info->xPos);
		xy[0].y = (Sint16)MTH_FixedToInt(info->yPos);
		//the way scale works is by giving the x/y coordinates of the top left and
		//bottom right corner of the sprite
		xy[1].x = (Sint16)(MTH_FixedToInt(MTH_Mul(info->xSize, info->scale) + info->xPos));
		xy[1].y = (Sint16)(MTH_FixedToInt(MTH_Mul(info->ySize, info->scale) + info->yPos));
		SPR_2ScaleSpr(0, info->mirror, COLOR_0, 0xffff, info->char_num, xy, NO_GOUR); //4bpp scaled sprite
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
		SPR_2DistSpr(0, info->mirror, COLOR_0, 0xffff, info->char_num, xy, NO_GOUR); //4bpp distorted sprite
	}
}

void sprite_make(int tile_num, Fixed32 x, Fixed32 y, SPRITE_INFO *ptr) {
	ptr->char_num = tile_num;
	ptr->options = 0;
	ptr->xPos = x;
	ptr->yPos = y;
	ptr->mirror = 0;
	ptr->dx = 0;
	ptr->dy = 0;
	ptr->scale = MTH_FIXED(1);
	ptr->angle = 0;
	ptr->animTimer = 0;
	ptr->animCursor = 0;
	ptr->iterate = NULL;
}

void sprite_draw_all() {
	int i;
	SPRITE_INFO tmp;
	for (i = 0; i < num_sprites; i++) {
		if (!(sprites[i].options & OPTION_NODISP) && sprites[i].iterate != NULL) {
			sprites[i].iterate(&sprites[i]);
		}
	}
	for (i = 0; i < num_sprites; i++) {
		if (!(sprites[i].options & OPTION_NODISP)) {
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
		if (sprites[i].options & OPTION_NODISP) {
			num_sprites++;
			sprites[i].index = i;
			sprites[i].iterate = NULL;
			return &sprites[i];
		}
	}
	return NULL;
}

void sprite_delete(SPRITE_INFO *sprite) {
	sprite->options |= OPTION_NODISP;
	sprite->iterate = NULL;
	num_sprites--;
}

void sprite_deleteall() {
	for (int i = 0; i < SPRITE_LIST_SIZE; i++) {
		sprites[i].options = OPTION_NODISP;
	}
	num_sprites = 0;
}

