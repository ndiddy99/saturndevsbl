#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"

extern SPRITE_INFO player;

void player_init();
void player_animate(void);
void player_input(void);
void player_draw(void);

#endif
