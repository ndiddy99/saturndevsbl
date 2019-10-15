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
    bomb->animTimer = FRAME_DELAY;
    bomb->iterate = &bomb_move;
}

void bomb_move(SPRITE_INFO *bomb) {
    if (bomb->animTimer == 0) {
        bomb->char_num++;
        if (bomb->char_num > BOMB_MAXCHARNUM) {
            //make sure explosion is centered over bomb
            explosion_make(bomb->xPos - MTH_FIXED(8), bomb->yPos - MTH_FIXED(8));
            sprite_delete(bomb);
            return;
        }
        bomb->animTimer = FRAME_DELAY;
    }
    else {
        bomb->animTimer--;
    }
}
