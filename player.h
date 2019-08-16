#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"

extern SPRITE_INFO player;

#define PLAYER_SPRITE_X (MTH_FIXED(144))
#define PLAYER_SPRITE_Y (MTH_FIXED(96))

void player_init();
void player_animate(void);
void player_input(void);
void player_draw(void);

#endif
