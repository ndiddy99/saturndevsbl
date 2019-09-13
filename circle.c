#include <sega_mth.h>

#include "circle.h"
#include "sprite.h"

#define CIRCLE_CHARNUM (20)
#define CIRCLE_SPEED MTH_FIXED(2)

void circle_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *circle = sprite_next();
    sprite_make(CIRCLE_CHARNUM, x, y, circle);
    circle->iterate = &circle_move;
}

//if there's area in front of where the circle is going, keep going
//if there's not, turn right
//if can't turn right, turn left
//if can't turn left, reverse direction
void circle_move(SPRITE_INFO *circle) {

}