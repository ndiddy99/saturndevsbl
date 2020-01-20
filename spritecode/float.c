#include <sega_mth.h>

#include "float.h"
#include "explosion.h"
#include "../sprite.h"
#include "../player.h"
#include "../graphicrefs.h"

#define FLOAT_CHARNUM (GRAPHIC_FLOAT)
#define FLOAT_MAXSPEED (MTH_FIXED(3))
#define FLOAT_ACCEL (MTH_FIXED(0.1))
#define FLOAT_STATE_DOWN (0)
#define FLOAT_STATE_UP (1)
#define FLOAT_SIZE (MTH_FIXED(32))

void float_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *float_spr = sprite_next();
    sprite_make(FLOAT_CHARNUM, x, y, float_spr);
    float_spr->animTimer = 0;
    float_spr->iterate = &float_move;
    float_spr->state = FLOAT_STATE_DOWN;
}

void float_move(SPRITE_INFO *float_spr) {
    if (float_spr->state == FLOAT_STATE_DOWN) {
        if (float_spr->dy > -FLOAT_MAXSPEED) {
            float_spr->dy -= FLOAT_ACCEL;
        }
        else {
            float_spr->dy = -FLOAT_MAXSPEED;
            float_spr->state = FLOAT_STATE_UP;
        }
    }
    else {
        if (float_spr->dy < FLOAT_MAXSPEED) {
            float_spr->dy += FLOAT_ACCEL;
        }
        else {
            float_spr->dy = FLOAT_MAXSPEED;
            float_spr->state = FLOAT_STATE_DOWN;
        }
    }
    float_spr->yPos += float_spr->dy;

    if (player.xPos > float_spr->xPos) {
            explosion_make(float_spr->xPos, float_spr->yPos);
            sprite_delete(float_spr);
        }    
}
