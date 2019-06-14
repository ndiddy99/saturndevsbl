#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"

extern SPRITE_INFO player;

void player_init();
void handle_player(SPRITE_INFO *player);
void handle_player_input(void);

#endif
