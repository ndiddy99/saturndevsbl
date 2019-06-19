#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "player.h"
#include "print.h"
#include "scroll.h"
#include "vblank.h"

const Uint16 states[] = {STATE_NULL, STATE_UP, STATE_DOWN, STATE_NULL,
	STATE_LEFT, STATE_UP, STATE_DOWN, STATE_LEFT, STATE_RIGHT, STATE_UP,
	STATE_DOWN, STATE_RIGHT, STATE_NULL, STATE_UP, STATE_DOWN, STATE_NULL};
const Uint16 player_down[] = {11, 10, 12, 10}; //frames for when the player's walking down
const Uint16 player_up[] = {14, 13, 15, 13};
const Uint16 player_side[] = {17, 16, 18, 16};
int anim_cursor = 0;
SPRITE_INFO player;

void player_init() {
	make_sprite(0, MTH_FIXED(144), MTH_FIXED(96), &player);
	player.charNum = 10;
}

void player_input() {
	Fixed32 scrollDX = 0;
	Fixed32 scrollDY = 0;
	Uint16 PadData1EW = PadData1E;
	PadData1E = 0;
	//if d-pad was pressed this frame, reset animation
	if (PadData1EW & 0xF000) {
		player.animTimer = 0;
	}
	player.state = states[(PadData1 >> 12)];
	
	if(PadData1 & PAD_U){
		scrollDY = MTH_FIXED(-2);
	}
	if(PadData1 & PAD_D){
		scrollDY = MTH_FIXED(2);
	}
	if (PadData1 & PAD_L) {
		scrollDX = MTH_FIXED(-2);
		
	}
	if (PadData1 & PAD_R) {
		scrollDX = MTH_FIXED(2);
	}
	player_animate(&player);
	move_scroll(0, scrollDX, scrollDY);
	print_num(scrolls_x[0] >> 16, 0, 0);
	print_num(scrolls_y[0] >> 16, 1, 0);
	print_num(player.charNum, 2, 0);
}

void player_animate(SPRITE_INFO *player) {
	if (player->animTimer > 0) player->animTimer--;
	
	if (player->state == STATE_LEFT) {
		player->mirror = MIRROR_HORIZ;
	}
	else if (player->state != STATE_NULL) {
		player->mirror = 0;
	}
	
	if (player->animTimer == 0) {
		player->animTimer = 10;
		switch (player->state) {
			case STATE_UP:
				player->charNum = player_up[anim_cursor];
			break;
			case STATE_DOWN:
				player->charNum = player_down[anim_cursor];
			break;
			case STATE_LEFT:
				player->charNum = player_side[anim_cursor];
			break;
			case STATE_RIGHT:
				player->charNum = player_side[anim_cursor];
			break;
		}
		anim_cursor == 3 ? anim_cursor = 0 : anim_cursor++;
	}
}
