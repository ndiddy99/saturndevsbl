#ifndef SPRITE_H
#define SPRITE_H

#include <sega_def.h>
#include <sega_mth.h>

#define MIRROR_HORIZ (1 << 4)
#define MIRROR_VERT (1 << 5)

typedef struct SpriteInfo {
	Uint16 charNum;
	Fixed32 xPos;
	Fixed32 yPos;
	Uint16 mirror;
	Uint16 xSize;
	Uint16 ySize;
	Fixed32 scale;
	Fixed32 angle;
} SPRITE_INFO;

//automatically picks the simplest SBL function for drawing the sprite depending
//on required features
//needs command to be opened before calling
void drawSprite(SPRITE_INFO *info);
void makeSprite(int spriteNum, Fixed32 x, Fixed32 y, SPRITE_INFO *ptr);

#endif
