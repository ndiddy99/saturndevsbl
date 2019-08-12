#ifndef SPRITE_H
#define SPRITE_H

#include <sega_def.h>
#include <sega_mth.h>

#define MIRROR_HORIZ (1 << 4)
#define MIRROR_VERT (1 << 5)

enum state {
	STATE_NODISP = 0,
	STATE_NULL,
	STATE_UP,
	STATE_DOWN,
	STATE_LEFT,
	STATE_RIGHT,
	STATE_UPLEFT,
	STATE_UPRIGHT,
	STATE_DOWNLEFT,
	STATE_DOWNRIGHT
};

typedef struct SpriteInfo {
	Uint16 char_num;
	Fixed32 xPos;
	Fixed32 yPos;
	Uint16 mirror;
	Fixed32 xSize;
	Fixed32 ySize;
	Fixed32 scale;
	Fixed32 angle;
	Uint16 animTimer;
	Uint16 state;
} SPRITE_INFO;

#define SPRITE_LIST_SIZE 200
SPRITE_INFO sprites[SPRITE_LIST_SIZE];
extern int num_sprites;

//automatically picks the simplest SBL function for drawing the sprite depending
//on required features
//needs command to be opened before calling
void sprite_draw(SPRITE_INFO *info);
void sprite_make(int spriteNum, Fixed32 x, Fixed32 y, SPRITE_INFO *ptr);
SPRITE_INFO *sprite_next(void);
void sprite_delete(int index);


#endif
