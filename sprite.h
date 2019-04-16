#ifndef SPRITE_H
#define SPRITE_H

#include <sega_def.h>
#include <sega_mth.h>

typedef struct SpriteInfo {
	Uint16 charNum;
	Fixed32 xPos;
	Fixed32 yPos;
	Uint16 xSize;
	Uint16 ySize;
	Fixed32 scale;
	Fixed32 angle;
} SPRITE_INFO;

//automatically picks the simplest SBL function for drawing the sprite depending
//on required features
//needs command to be opened before calling
void drawSprite(SPRITE_INFO *info);

#endif
