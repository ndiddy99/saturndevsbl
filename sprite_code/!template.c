#include <sega_mth.h>

#include "!template.h"
#include "../sprite.h"
#include "../player.h"

#define TEMPLATE_CHARNUM (0)

void template_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *template = sprite_next();
    sprite_make(TEMPLATE_CHARNUM, x, y, template);
    template->speed = 0;
    template->animTimer = 0;
    template->iterate = &template_move;
}

void template_move(SPRITE_INFO *template) {

}
