#include <sega_mth.h>

#include "bomber.h"
#include "bomb.h"
#include "../sprite.h"
#include "../player.h"

#define BOMBER_CHARNUM (56)
#define BOMBER_MAXSPEED (MTH_FIXED(1.5))
#define BOMBER_ACCEL (MTH_FIXED(0.2))
//frames between placing bombs
#define BOMBING_INTERVAL (60)

void bomber_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *bomber = sprite_next();
    sprite_make(BOMBER_CHARNUM, x, y, bomber);
    bomber->animTimer = 0;
    bomber->iterate = &bomber_move;
}

void bomber_move(SPRITE_INFO *bomber) {
    Fixed32 x_diff = (player.xPos + (player.xSize >> 1)) - (bomber->xPos + (bomber->xSize >> 1));
    Fixed32 y_diff = (player.yPos + (player.ySize >> 1)) - (bomber->yPos + (bomber->ySize >> 1));
    Fixed32 target_angle = MTH_Atan(y_diff, x_diff);
    bomber->dx += MTH_Mul(MTH_Cos(target_angle), BOMBER_ACCEL);
    if (bomber->dx > BOMBER_MAXSPEED) { bomber->dx = BOMBER_MAXSPEED; }
    else if (bomber->dx < -BOMBER_MAXSPEED) { bomber->dx = -BOMBER_MAXSPEED; }
    bomber->dy += MTH_Mul(MTH_Sin(target_angle), BOMBER_ACCEL);
    if (bomber->dy > BOMBER_MAXSPEED) { bomber->dy = BOMBER_MAXSPEED; }
    else if (bomber->dy < -BOMBER_MAXSPEED) { bomber->dy = -BOMBER_MAXSPEED; }
    bomber->xPos += bomber->dx;
    bomber->yPos += bomber->dy;

    //place a bomb every BOMBING_INTERVAL frames
    bomber->animTimer++;
    if (bomber->animTimer == BOMBING_INTERVAL) {
        bomb_make(bomber->xPos + MTH_FIXED(8), bomber->yPos + MTH_FIXED(8));
        bomber->animTimer = 0;
    }
}
