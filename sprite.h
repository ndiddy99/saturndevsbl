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

struct SpriteInfo;

typedef void (*IterateFunc)(struct SpriteInfo *);

typedef struct SpriteInfo {
	Uint16 char_num; //tile number
	Uint16 index; //where the sprite is in the sprites array
	Fixed32 xPos;
	Fixed32 yPos;
	Fixed32 xSize;
	Fixed32 ySize;
	Fixed32 speed; //sprite's max speed
	Fixed32 scale;
	Fixed32 angle;
	Uint16 mirror;
	Uint16 animTimer; //timer for animations
	Uint16 state; //where the sprite is moving
	Uint16 facing; //where the sprite is facing
	IterateFunc iterate;
} SPRITE_INFO;

//sets up initial sprite display
void sprite_init(void);
//automatically picks the simplest SBL function for drawing the sprite depending
//on required features
//needs command to be opened before calling
void sprite_draw(SPRITE_INFO *info);
//inits the SPRITE_INFO pointer given
void sprite_make(int tile_num, Fixed32 x, Fixed32 y, SPRITE_INFO *ptr);
//draws all sprites in the sprite list
void sprite_draw_all(void);
//gets a pointer to the next sprite in the list
SPRITE_INFO *sprite_next(void);
//deletes the given sprite from the sprite list
void sprite_delete(SPRITE_INFO *sprite);
//utility function that moves a sprite based on its state
//collision: 1 to do collision, 0 to not
//returns the tile the sprite is over
Uint16 sprite_move(SPRITE_INFO *sprite, int collision);

#endif
