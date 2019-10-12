#include <sega_mth.h>

#include "explosion.h"
#include "../sprite.h"
#include "../player.h"

#define EXPLOSION_MINCHARNUM (48)
#define EXPLOSION_MAXCHARNUM (51)
//how many frames to wait before switching to next frame
#define FRAME_DELAY (5)

void explosion_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *explosion = sprite_next();
    sprite_make(EXPLOSION_MINCHARNUM, x, y, explosion);
    explosion->speed = 0;
    explosion->animTimer = FRAME_DELAY;
    explosion->iterate = &explosion_move;
}

void explosion_move(SPRITE_INFO *explosion) {
    if (explosion->animTimer == 0) {
        explosion->char_num++;
        if (explosion->char_num > EXPLOSION_MAXCHARNUM) {
            sprite_delete(explosion);
            return;
        }
        explosion->animTimer = FRAME_DELAY;
    }
    else {
        explosion->animTimer--;
    }
}
