#include <sega_def.h>
#include "player.h"

const Uint16 player_down[] = {1, 0, 2, 0}; //frames for when the player's walking down
const Uint16 player_up[] = {4, 3, 5, 3};
static int anim_cursor = 0;

void handle_player(SPRITE_INFO *player) {
	if (player->state == STATE_STILL)
		player->charNum = 0;
	else {
		player->animTimer++;
		if (player->animTimer == 10) {
			player->animTimer = 0;
			if (player->state == STATE_UP)
				player->charNum = player_up[anim_cursor];
			if (player->state == STATE_DOWN)
				player->charNum = player_down[anim_cursor];
			anim_cursor == 3 ? anim_cursor = 0 : anim_cursor++;
		}
	}
}
