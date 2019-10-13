#include <sega_mth.h>

#include "bomb.h"
#include "explosion.h"
#include "../sprite.h"
#include "../player.h"

#define BOMB_MINCHARNUM (52)
#define BOMB_MAXCHARNUM (55)
#define FRAME_DELAY (5)

void bomb_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *bomb = sprite_next();
    sprite_make(BOMB_MINCHARNUM, x, y, bomb);
    bomb->speed = 0;
    bomb->animTimer = FRAME_DELAY;
    bomb->iterate = &bomb_move;
}

void bomb_move(SPRITE_INFO *bomb) {
    if (bomb->animTimer == 0) {
        bomb->char_num++;
        if (bomb->char_num > BOMB_MAXCHARNUM) {
            explosion_make(bomb->xPos, bomb->yPos);
            sprite_delete(bomb);
            return;
        }
        bomb->animTimer = FRAME_DELAY;
    }
    else {
        bomb->animTimer--;
    }
}
