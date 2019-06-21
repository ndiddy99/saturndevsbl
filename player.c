#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "player.h"
#include "print.h"
#include "scroll.h"
#include "vblank.h"

#define PLAYER_SPEED (2)
#define DIAGONAL_MULTIPLIER (0.7071) //square root of 1/2, normalizes diagonal speed
#define PLAYER_SPRITE_X 144
#define PLAYER_SPRITE_Y 96

const Uint16 states[] = {STATE_NULL, STATE_UP, STATE_DOWN, STATE_NULL,
	STATE_LEFT, STATE_UP, STATE_DOWN, STATE_LEFT, STATE_RIGHT, STATE_UP,
	STATE_DOWN, STATE_RIGHT, STATE_NULL, STATE_UP, STATE_DOWN, STATE_NULL};
const int diagonals[] = {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0};
const Uint16 player_down[] = {11, 10, 12, 10}; //frames for when the player's walking down
const Uint16 player_up[] = {14, 13, 15, 13};
const Uint16 player_side[] = {17, 16, 18, 16};
int anim_cursor = 0;
SPRITE_INFO player;

void player_init() {
	make_sprite(0, MTH_FIXED(PLAYER_SPRITE_X), MTH_FIXED(PLAYER_SPRITE_Y), &player);
	player.char_num = 10;
	set_scroll(0, MTH_FIXED(48), MTH_FIXED(16));
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
		scrollDY = MTH_FIXED(-PLAYER_SPEED);
	}
	if(PadData1 & PAD_D){
		scrollDY = MTH_FIXED(PLAYER_SPEED);
	}
	if (PadData1 & PAD_L) {
		scrollDX = MTH_FIXED(-PLAYER_SPEED);
		
	}
	if (PadData1 & PAD_R) {
		scrollDX = MTH_FIXED(PLAYER_SPEED);
	}
	if (diagonals[PadData1 >> 12]) {
		MTH_Mul(scrollDX, MTH_FIXED(DIAGONAL_MULTIPLIER));
		MTH_Mul(scrollDY, MTH_FIXED(DIAGONAL_MULTIPLIER));
	}
	
	player_animate(&player);
	move_scroll(0, scrollDX, scrollDY);
	print_num(scrolls_x[0] >> 16, 0, 0);
	print_num(scrolls_y[0] >> 16, 1, 0);
	int tile_x = MTH_FixedToInt(scrolls_x[0] + MTH_FIXED(PLAYER_SPRITE_X)) >> 4;
	int tile_y = MTH_FixedToInt(scrolls_y[0] + MTH_FIXED(PLAYER_SPRITE_Y)) >> 4;
	print_num(get_map_val(0, tile_x, tile_y), 2, 0);
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
				player->char_num = player_up[anim_cursor];
			break;
			case STATE_DOWN:
				player->char_num = player_down[anim_cursor];
			break;
			case STATE_LEFT:
				player->char_num = player_side[anim_cursor];
			break;
			case STATE_RIGHT:
				player->char_num = player_side[anim_cursor];
			break;
		}
		anim_cursor == 3 ? anim_cursor = 0 : anim_cursor++;
	}
}
