#include <sega_mth.h>

#include "template.h"
#include "../sprite.h"
#include "../player.h"

#define ENEMY_CHARNUM (0)

void enemy_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *enemy = sprite_next();
    sprite_make(ENEMY_CHARNUM, x, y, enemy);
    enemy->speed = 0;
    enemy->animTimer = FRAME_DELAY;
    enemy->iterate = &enemy_move;
}

void enemy_move(SPRITE_INFO *enemy) {

}
