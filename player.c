#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include <string.h>
#include "collision.h"
#include "player.h"
#include "print.h"
#include "scroll.h"
#include "sprite.h"
#include "vblank.h"

#define PLAYER_ACCEL (MTH_FIXED(0.5))
#define PLAYER_JUMPSPEED (-MTH_FIXED(8))
#define PLAYER_MAXSPEED (MTH_FIXED(4))
#define PLAYER_MAXXPOS (MTH_FIXED(152))
//double jump
#define PLAYER_MAXJUMPS (2)

//boost stuff
#define BOOST_TIMER (60)
#define BOOST_SPEED (MTH_FIXED(10))

#define FRAME_STAND (96)
#define FRAME_WALK1 (FRAME_STAND + 1)
#define FRAME_WALK2 (FRAME_STAND + 2)
const Uint16 player_frames[] = {FRAME_WALK1, FRAME_STAND, FRAME_WALK2, FRAME_STAND};
SPRITE_INFO player;
Uint8 boost = 0;
Uint8 jumps = 0;

void player_init() {
	sprite_make(FRAME_STAND, MTH_FIXED(20), MTH_FIXED(16) + PLAYER_SPRITE_Y, &player);
	player.xSize = MTH_FIXED(16);
	player.ySize = MTH_FIXED(32);
}

void player_input() {
	Uint16 PadData1EW = PadData1E;
	PadData1E = 0;
	if (PadData1 & PAD_L) {
		if (player.dx > -PLAYER_MAXSPEED) {
			player.dx -= PLAYER_ACCEL;
		}
		else {
			player.dx += PLAYER_ACCEL;
		}
		player.mirror = MIRROR_HORIZ;
	}
	else if (PadData1 & PAD_R) {
		if (player.dx < PLAYER_MAXSPEED) {
			player.dx += PLAYER_ACCEL;
		}
		else {
			player.dx -= PLAYER_ACCEL;
		}
		player.mirror &= ~MIRROR_HORIZ;
	}
	else {
		if (player.dx > 0) {
			player.dx -= PLAYER_ACCEL;
		}
		else if (player.dx < 0) {
			player.dx += PLAYER_ACCEL;
		}		
	}
	player.xPos += player.dx;
	player_animate();	
	collision_eject_horiz(&player);

	//jump button
	if (collision_check_below(&player)) {
		jumps = 0;
	}
	if ((PadData1EW & PAD_B) && jumps < PLAYER_MAXJUMPS) {
		player.dy = -MTH_FIXED(8);
		jumps++;
	}
	//if you hold the jump button longer, jump higher
	else if ((PadData1 & PAD_B) && player.dy < 0) {
		player.dy += MTH_FIXED(0.25);
	}
	else {
		player.dy += SPRITE_GRAVITY;
	}
	player.yPos += player.dy;
	collision_eject_vert(&player);

	//boost button
	if ((PadData1EW & PAD_A) && boost == 0) {
		boost = BOOST_TIMER;
		if (PadData1 & PAD_L) {
			player.dx -= BOOST_SPEED;
		}
		if (PadData1 & PAD_R) {
			player.dx += BOOST_SPEED;
		}
	}
	if (boost) { 
		boost--;
	}

	print_string("x: ", 2, 0); print_num(player.xPos >> 16, 2, 4);
	print_string("y: ", 3, 0); print_num(player.yPos >> 16, 3, 4);
	print_string("dx: ", 4, 0); print_num(player.dx, 4, 4);
	print_string("dy: ", 5, 0); print_num(player.dy, 5, 4);
	print_string("boost: ", 6, 0); print_num(boost, 6, 7);
}

void player_animate() {
	if (player.dx) {
		player.animTimer++;
		if (player.animTimer >= 10) {
			player.animTimer = 0;
			player.animCursor++;
			if (player.animCursor > 3) { player.animCursor = 0; }
			player.char_num = player_frames[player.animCursor];
		}
	}
	else {
		player.char_num = FRAME_STAND;
		player.animTimer = 10;
	}
}
//allows me to treat the player sprite like any other sprite while only moving the screen
//around it
void player_draw() {
	SPRITE_INFO temp;
	memcpy(&temp, &player, sizeof(temp));
	if (temp.xPos > PLAYER_MAXXPOS) {
		scroll_set(SCROLL_PLAYFIELD, temp.xPos - PLAYER_MAXXPOS, 0);
		temp.xPos = PLAYER_MAXXPOS;
	}
	else {
		scroll_set(SCROLL_PLAYFIELD, 0, 0);
	}
	// scroll_move(SCROLL_PLAYFIELD, MTH_FIXED(1), MTH_FIXED(0));

	sprite_draw(&temp);
}
