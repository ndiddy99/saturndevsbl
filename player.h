#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"

extern SPRITE_INFO player;

#define PLAYER_SPRITE_X (MTH_FIXED(144))
#define PLAYER_SPRITE_Y (MTH_FIXED(96))

void player_init();
void player_animate(void);
//returns 1 if the player can kill whatever sprite he's touching
inline int player_cankill();
//run when the player kills an enemy
inline void player_killenemy();
//run to kill the player
void player_die();
void player_input(void);
void player_draw(void);

#endif
