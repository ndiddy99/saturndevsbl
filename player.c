#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#define		_SPR2_
#include <SEGA_SPR.H>
#include <sega_sys.h>
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
#define FRAME_JUMP1 (FRAME_STAND + 3)
#define FRAME_JUMP2 (FRAME_STAND + 4)
const Uint16 player_frames[] = {FRAME_WALK1, FRAME_STAND, FRAME_WALK2, FRAME_STAND};

SPRITE_INFO player;
LEVEL *curr_level;
Uint8 boost = 0;
Uint8 jumps = 0;
Uint8 lives = 3;

SclLineparam lp;

void player_init(LEVEL *level) {
	sprite_make(FRAME_STAND, level->player_startx, level->player_starty, &player);
	player.xSize = MTH_FIXED(16);
	player.ySize = MTH_FIXED(32);
	curr_level = level;
	
	// int i;
	// for (i = 0; i < 224; i++) {
	// 	if (i > 128) {
	// 		lp.line_tbl[i].h = MTH_FIXED(i);
	// 	}
	// }
}

//run when the player dies
static void player_kill() {
	player.xPos = curr_level->player_startx;
	player.yPos = curr_level->player_starty;
	player.dx = 0;
	player.dy = 0;
	jumps = 0;
	boost = 0;
	lives--;
	scroll_reset();
}

void player_input() {
	//this is cargo culting from sega's code, maybe there's some reason why i can't read PadData1E more than once?
	Uint16 PadData1EW = PadData1E;
	PadData1E = 0;
	//soft reset
	if (PadData1 == (PAD_A | PAD_B | PAD_C | PAD_S)) {
		//go back to the cd player screen if a+b+c+start is pressed
		SYS_EXECDMP();
	}

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
	player.xPos += player.dx;

	if (PadData1EW & PAD_C) {
		player.xPos += MTH_FIXED(1);
	}

	player_animate();
	collision_eject_horiz(&player);

	//jump button
	//if we're falling and on a solid surface or a slope
	if ((player.dy > 0) && ((player.options & OPTION_SLOPE) || collision_check_below(&player))) {
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
		if (player.dy > SPRITE_MAXFALLSPEED) {
			player.dy = SPRITE_MAXFALLSPEED;
		}
		
	}
	player.yPos += player.dy;
	//if the player lands on/jumps into spikes, kill him
	if (collision_spikes(&player)) {
		player_kill();
	}
	collision_eject_vert(&player);
	//if the player falls into a pit, kill him
	if (player.yPos > MTH_FIXED(224)) {
		player_kill();
	}
	print_string("x: ", 2, 0); print_num(player.xPos >> 16, 2, 4); print_num(player.xPos & 0xffff, 2, 14);
	print_string("y: ", 3, 0); print_num(player.yPos >> 16, 3, 4); print_num(player.yPos & 0xffff, 3, 14);
	print_string("dx: ", 4, 0); print_num(player.dx, 4, 4);
	print_string("dy: ", 5, 0); print_num(player.dy, 5, 4);
	print_string("boost: ", 6, 0); print_num(boost, 6, 7);
	print_string("lives: ", 7, 0); print_num(lives, 7, 7);
	print_num(PadData1, 8, 0);
}

void player_animate() {
	if (jumps == 1) {
		player.char_num = FRAME_JUMP1;
		player.animTimer = 10;
	}
	else if (jumps == 2) {
		player.char_num = FRAME_JUMP2;
		player.animTimer = 10;		
	}
	else if (player.dx) {
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
//around him
void player_draw() {
	SPRITE_INFO temp;
	Fixed32 bg_scroll_val;
	memcpy(&temp, &player, sizeof(temp));
	if (temp.xPos > PLAYER_MAXXPOS) {
		Fixed32 newXPos = temp.xPos - PLAYER_MAXXPOS;
		scroll_set(SCROLL_PLAYFIELD, newXPos, 0);
		bg_scroll_val = newXPos >> 1;
		temp.xPos = PLAYER_MAXXPOS;
	}
	else {
		scroll_set(SCROLL_PLAYFIELD, 0, 0);
		bg_scroll_val = 0;
	}
	scroll_linescroll4(SCROLL_BACKGROUND1, bg_scroll_val, 45, 78, 114);
	scroll_move(SCROLL_BACKGROUND2, MTH_FIXED(1) + (temp.dx >> 3), 0);
	// scroll_move(SCROLL_PLAYFIELD, MTH_FIXED(1), MTH_FIXED(0));
	sprite_draw(&temp);
}
