#include <sega_def.h>
#include <sega_mth.h>
#define _SPR2_
#include <sega_spr.h>

#include "sprite.h"

void drawSprite(SPRITE_INFO *info) {
	XyInt xy[4];
	Fixed32 xOffset, yOffset, sin, cos, scaledX, scaledY;
	int i;
	
	if (info->scale == MTH_FIXED(1) && info->angle == 0) {
		xy[0].x = (Sint16)MTH_FixedToInt(info->xPos);
		xy[0].y = (Sint16)MTH_FixedToInt(info->yPos);
		SPR_2NormSpr(0, info->mirror, COLOR_5, 0, info->charNum, xy, NO_GOUR); //rgb normal sprite
	}
	
	else if (info->angle == 0){
		xy[0].x = (Sint16)MTH_FixedToInt(info->xPos);
		xy[0].y = (Sint16)MTH_FixedToInt(info->yPos);
		//the way scale works is by giving the x/y coordinates of the top left and
		//bottom right corner of the sprite
		xy[1].x = (Sint16)(MTH_FixedToInt(MTH_Mul(MTH_IntToFixed(info->xSize), info->scale) + info->xPos));
		xy[1].y = (Sint16)(MTH_FixedToInt(MTH_Mul(MTH_IntToFixed(info->ySize), info->scale) + info->yPos));
		SPR_2ScaleSpr(0, info->mirror, COLOR_5, 0, info->charNum, xy, NO_GOUR); //rgb scaled sprite
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
		SPR_2DistSpr(0, info->mirror, COLOR_5, 0, info->charNum, xy, NO_GOUR); //rgb distorted sprite
	}
}
