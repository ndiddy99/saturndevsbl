#include <sega_mth.h>

#include "hurler.h"
#include "../sprite.h"
#include "../player.h"

static const Uint16 hurler_chars[] = {58, 57, 59, 57};
#define HURLER_ANIMTIME (10)

void hurler_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *hurler = sprite_next();
    sprite_make(hurler_chars[0], x, y, hurler);
    hurler->speed = 0;
    hurler->animTimer = 0;
    hurler->iterate = &hurler_move;
}

void hurler_move(SPRITE_INFO *hurler) {
    if (hurler->animTimer++ > HURLER_ANIMTIME) {
        hurler->animTimer = 0;
        hurler->char_num = hurler_chars[hurler->animCursor];
        hurler->animCursor == 3 ? hurler->animCursor = 0 : hurler->animCursor++; //3 is size of hurler_chars array
    }
}
