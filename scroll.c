#include <string.h>
#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>

#include "cd.h"
#include "enemylist.h"
#include "graphicrefs.h"
#include "player.h"
#include "print.h"
#include "scroll.h"
#include "sprite.h"

Fixed32 scrolls_x[]  = {0, 0, 0, 0};
Fixed32 scrolls_y[]  = {0, 0, 0, 0};
Sint32 map_tiles_x[] = {0, 0};
Sint32 map_tiles_y[] = {0, 0};
Uint32 copy_modes[]  = {0, 0};
Uint16 *maps[2]; //map locations in WRAM for the scrolling backgrounds
int scroll_xsize = 0;
int scroll_ysize = 0;
Uint32 vram[] = {SCL_VDP2_VRAM_A0, SCL_VDP2_VRAM_A0 + 0x800, SCL_VDP2_VRAM_B1, SCL_VDP2_VRAM_B1 + 0x800}; //where in VRAM each tilemap is
#define VRAM_PTR(bg) ((Uint16 *)vram[bg])
int scroll_transition_state = TSTATE_NULL;

/*
 * 0: NBG0 Pattern Name
 * 1: NBG1 Pattern Name
 * 2: NBG2 Pattern Name
 * 3: NBG3 Pattern Name
 * 4: NBG0 Character Pattern
 * 5: NBG1 Character Pattern
 * 6: NBG2 Character Pattern
 * 7: NBG3 Character Pattern
 * C: NBG0 Vertical Scroll Table
 * D: NBG1 Vertical Scroll Table
 * E: CPU Read/Write
 * F: No Access
 */

/*
 Data Type			# Accesses required
 Pattern name data          1
 16-color tiles		  		1
 256-color tiles	  		2
 2048-color tiles	  		4
 32K-color tiles	  		4
 16M-color tiles	  		8
 Vertical scroll data 		1
 */

// There's also numerous read restrictions, see SOA technical bulletin #6 for more information

Uint16	CycleTb[]={
	0x0011,0xeeee,
	0x5555,0x4444,
	0x6677,0xffff,
	0x23ff,0xeeee
};

SclConfig scfg0;
SclConfig scfg1;
SclConfig scfg2;
SclConfig scfg3;

SclLineparam line_param0;
SclLineparam line_param1;

LEVEL *curr_level;

void scroll_init(LEVEL *level) {
	int i, j, count;
	Uint16 BackCol;
	Uint8 *VramWorkP;
	Uint16 *TilemapVram;
	Uint16 *lwram_ptr = (Uint16 *)LWRAM;
	SclVramConfig vram_cfg;

	SCL_SetColRamMode(SCL_CRM24_1024);
		SCL_AllocColRam(SCL_NBG2, 256, OFF);
		SCL_SetColRam(SCL_NBG2, 0, 256, (void *)(level->playfield.palette));

		SCL_AllocColRam(SCL_NBG0, 16, OFF);
		SCL_SetColRam(SCL_NBG0, 0, 16, (void *)(level->bg_far.palette));		

		SCL_AllocColRam(SCL_NBG1, 16, OFF);
		SCL_SetColRam(SCL_NBG1, 0, 16, (void *)(level->bg_near.palette));

		BackCol = 0x0000; //set the background color to black
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	//---nbg0: far bg---
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_A1;
	cd_load(level->bg_far.tile_name, (void *)LWRAM, 128 * level->bg_far.tile_num);
	memcpy(VramWorkP, (void *)LWRAM, 128 * level->bg_far.tile_num);
	cd_load(level->bg_far.map_name, (void *)LWRAM, level->bg_far.map_width * level->bg_far.map_height * 2);
	TilemapVram = VRAM_PTR(0);
	memcpy(TilemapVram, (void *)LWRAM, level->bg_far.map_width * level->bg_far.map_height * 2);

	SCL_InitLineParamTb(&line_param0);
	line_param0.h_enbl = ON;
	line_param0.line_addr = SCL_VDP2_VRAM_A1 + 61440; //60kb into A1
	line_param0.interval = SCL_1_LINE;

	//nbg1: near bg
	VramWorkP = (Uint8 *)(SCL_VDP2_VRAM_A1 + 65536); //64kb into A1
	cd_load(level->bg_near.tile_name, (void *)LWRAM, 128 * level->bg_near.tile_num);
	memcpy(VramWorkP, (void *)LWRAM, 128 * level->bg_near.tile_num);
	cd_load(level->bg_near.map_name, (void *)LWRAM, level->bg_near.map_width * level->bg_near.map_height * 2);
	TilemapVram = VRAM_PTR(1);
	//copy tiles to vram, OR'ing with 512 to make the tilemap line up with the tiles referenced
	for (i = 0; i < level->bg_near.map_width * level->bg_near.map_height; i++) {
		TilemapVram[i] = lwram_ptr[i] | 512;
	}

	//---nbg2: playfield---
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B0; //bg character pattern to vram b0
	cd_load(level->playfield.tile_name, (void *)LWRAM, 256 * level->playfield.tile_num);
	memcpy(VramWorkP, (void *)LWRAM, 256 * level->playfield.tile_num);
	scroll_xsize = level->playfield.map_width;
	scroll_ysize = level->playfield.map_height;	
	//load playfield map data to LWRAM
	cd_load(level->playfield.map_name, (void *)LWRAM, scroll_xsize * scroll_ysize * 2);
	count = 0;
	TilemapVram = VRAM_PTR(2);
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 32; j++) {
			TilemapVram[count++] = lwram_ptr[i * scroll_xsize + j];
		}
	}
	maps[SCROLL_PLAYFIELD] = (Uint16 *)LWRAM;
	// memcpy(TilemapVram, (void *)LWRAM, 0x800);
	// TilemapVram[0] = 2;
	// TilemapVram[1] = 2;

	// TilemapVram = VRAM_PTR(3);
	// memcpy(TilemapVram, data->bg3_tilemap, 0x800);

	//scroll initial configuration
	SCL_InitConfigTb(&scfg0);
		scfg0.dispenbl      = ON;
		scfg0.charsize      = SCL_CHAR_SIZE_2X2;
		scfg0.pnamesize     = SCL_PN1WORD;
		scfg0.flip          = SCL_PN_10BIT;
		scfg0.platesize     = SCL_PL_SIZE_1X1; //they meant "plane size"
		scfg0.coltype       = SCL_COL_TYPE_16;
		scfg0.datatype      = SCL_CELL;
		scfg0.patnamecontrl = 0x0004; //vram A1 offset
	for(i=0;i<4;i++)   scfg0.plate_addr[i] = vram[0];
	SCL_SetConfig(SCL_NBG0, &scfg0);

	memcpy((void *)&scfg1, (void *)&scfg0, sizeof(SclConfig));
	scfg1.patnamecontrl = 0x0004;
	scfg1.dispenbl = ON;
	for(i=0;i<4;i++)   scfg1.plate_addr[i] = vram[1];
	SCL_SetConfig(SCL_NBG1, &scfg1);

	memcpy((void *)&scfg2, (void *)&scfg0, sizeof(SclConfig));
	scfg2.dispenbl = ON;
	scfg2.coltype = SCL_COL_TYPE_256;
	scfg2.patnamecontrl = 0x0008;
	for(i=0;i<4;i++)   scfg2.plate_addr[i] = vram[2];
	SCL_SetConfig(SCL_NBG2, &scfg2);

	memcpy((void *)&scfg3, (void *)&scfg2, sizeof(SclConfig));
	scfg3.dispenbl = OFF;
	for(i=0;i<4;i++)   scfg3.plate_addr[i] = vram[3];
	SCL_SetConfig(SCL_NBG3, &scfg3);
	
	//setup VRAM configuration
	SCL_InitVramConfigTb(&vram_cfg);
		vram_cfg.vramModeA = ON; //separate VRAM A into A0 & A1
		vram_cfg.vramModeB = ON; //separate VRAM B into B0 & B1
	SCL_SetVramConfig(&vram_cfg);

	//setup vram access pattern
	SCL_SetCycleTable(CycleTb);
	 
	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(0), FIXED(0), 0); //home position
	SCL_Close();
	SCL_Open(SCL_NBG1);
		SCL_MoveTo(FIXED(0), FIXED(0), 0);
	SCL_Close();
	SCL_Open(SCL_NBG2);
		SCL_MoveTo(FIXED(0), FIXED(0), 0);
	SCL_Close();
	SCL_Open(SCL_NBG3);
		SCL_MoveTo(FIXED(0), FIXED(0), 0);
	SCL_Close();
	scroll_scale(0, FIXED(1));
	scroll_scale(1, FIXED(1));
	SCL_SetPriority(SCL_NBG3, 7); //set layer priorities
	SCL_SetPriority(SCL_SPR,  6);
	SCL_SetPriority(SCL_NBG2, 6);
	SCL_SetPriority(SCL_NBG1, 5);
	SCL_SetPriority(SCL_NBG0, 4);
	
	curr_level = level;
}

void scroll_move(int num, Fixed32 x, Fixed32 y) {
	Sint32 curr_tile_x;
	scrolls_x[num] += x;
	//playfield and foreground use flip-style scrolling, so don't change
	//y position here for them
	if (num > 2) {
		scrolls_y[num] += y;
	}
	if (num < 2) {
		curr_tile_x = MTH_FixedToInt(scrolls_x[num]) >> 4; //tile size is 16x16
		// curr_tile_y = MTH_FixedToInt(scrolls_y[num]) >> 4;
		// copy_modes[num] = 0;
		if (curr_tile_x - map_tiles_x[num] > 0) { //if x value increasing
			copy_modes[num] |= COPY_MODE_RCOL;
		}
		else if (curr_tile_x - map_tiles_x[num] < 0) { //if x value decreasing
			copy_modes[num] |= COPY_MODE_LCOL;
		}
		// if (curr_tile_y - map_tiles_y[num] > 0) { //if y value increasing
		// 	copy_modes[num] |= COPY_MODE_BROW;
		// }
		// else if (curr_tile_y - map_tiles_y[num] < 0) { //if y value decreasing
		// 	copy_modes[num] |= COPY_MODE_TROW;
		// }
		map_tiles_x[num] = curr_tile_x;
		// map_tiles_y[num] = curr_tile_y;
	}
	switch (num) {
		case SCROLL_PLAYFIELD:
			SCL_Open(SCL_NBG2);
			break;
		case SCROLL_FOREGROUND:
			SCL_Open(SCL_NBG3);
			break;
		case SCROLL_BACKGROUND1:
			SCL_Open(SCL_NBG0);
			break;
		case SCROLL_BACKGROUND2:
			SCL_Open(SCL_NBG1);
			break;
	}
		if (num < 2) {
			SCL_MoveTo(scrolls_x[num], 0, 0);
		}
		else {
			SCL_MoveTo(scrolls_x[num], scrolls_y[num], 0);
		}
	SCL_Close();
}

//moves scroll absolutely to coordinates
void scroll_set(int num, Fixed32 x, Fixed32 y) {
	scroll_move(num, x - scrolls_x[num], y - scrolls_y[num]);
}

#define ZOOM_HALF_NBG0 (0x1)
#define ZOOM_QUARTER_NBG0 (0x2)
#define ZOOM_HALF_NBG1 (0x100)
#define ZOOM_QUARTER_NBG1 (0x200)
#define LOW_BYTE (0xFF)
#define HIGH_BYTE (0xFF00)

void scroll_scale(int num, Fixed32 scale) {
	SCL_Open(1 << (num + 2));
		SCL_Scale(scale, scale);
	SCL_Close();
	//reset the configuration byte for the given background
	Scl_n_reg.zoomenbl &= (num == 0 ? HIGH_BYTE : LOW_BYTE);
	if (scale >= FIXED(1)) {
		Scl_n_reg.zoomenbl &= (num == 0 ? ~(ZOOM_HALF_NBG0 | ZOOM_QUARTER_NBG0)
										: ~(ZOOM_HALF_NBG1 | ZOOM_QUARTER_NBG1));
		return;
	}
	else if (scale < FIXED(1) && scale >= FIXED(0.5)) {
		Scl_n_reg.zoomenbl |= (num == 0 ? ZOOM_HALF_NBG0 : ZOOM_HALF_NBG1);
	}
	else {
		Scl_n_reg.zoomenbl |= (num == 0 ? ZOOM_QUARTER_NBG0 : ZOOM_QUARTER_NBG1);
	}
}

//gets the value at the given coordinates for a map
Uint16 scroll_get(int map, int x, int y) {
	Uint16 *map_ptr = maps[map];
	if (map_ptr == NULL || x >= scroll_xsize || x < 0 || y >= scroll_ysize || y < 0) {
		return 0;
	}
	return map_ptr[(y * scroll_xsize) + x];
}

void scroll_copy(int num) {
	int i, pos;

	Uint16 *vram_ptr = VRAM_PTR(num + 2);
	if (copy_modes[num] & COPY_MODE_RCOL) {
		for (i = 0; i < 16; i++) {
			pos = (i * 32) + ((map_tiles_x[num] + SCREEN_TILES_X) % 32);
			if (pos >= 0) {
				vram_ptr[pos] = scroll_get(num, map_tiles_x[num] + SCREEN_TILES_X, i + map_tiles_y[num]);
			}
		}
	}
	if (copy_modes[num] & COPY_MODE_LCOL) {
		for (i = 0; i < 16; i++) {
			pos = (i * 32) + ((map_tiles_x[num] - 1) % 32);
			if (pos >= 0) {
				vram_ptr[pos] = scroll_get(num, map_tiles_x[num] - 1, i + map_tiles_y[num]);
			}
		}		
	}
	// if (copy_modes[num] & COPY_MODE_BROW) {
	// 	for (i = -1; i < SCREEN_TILES_X + 1; i++) {
	// 		pos = (((map_tiles_y[num] + SCREEN_TILES_Y) % 32) * 32) + ((i + map_tiles_x[num]) % 32);
	// 		if (pos >= 0) {
	// 			vram_ptr[pos] = scroll_get(num, map_tiles_x[num] + i, map_tiles_y[num] + SCREEN_TILES_Y);
	// 		}
	// 	}
	// }
	// if (copy_modes[num] & COPY_MODE_TROW) {
	// 	for (i = -1; i < SCREEN_TILES_X + 1; i++) {
	// 		pos = (((map_tiles_y[num] - 1) % 32) * 32) + ((i + map_tiles_x[num]) % 32);
	// 		if (pos >= 0) {
	// 			vram_ptr[(((map_tiles_y[num] - 1) % 32) * 32) + ((i + map_tiles_x[num]) % 32)] =
	// 				scroll_get(num, map_tiles_x[num] + i, map_tiles_y[num] - 1);
	// 		}
	// 	}
	// 
}

void scroll_reset(void) {
	int i, j, count;
	Uint16 *lwram_ptr;
	Uint16 *tilemap_vram;
	scroll_set(SCROLL_PLAYFIELD, 0, 0);
	count = 0;
	tilemap_vram = VRAM_PTR(2);
	lwram_ptr = maps[SCROLL_PLAYFIELD];
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 32; j++) {
			tilemap_vram[count++] = lwram_ptr[i * scroll_xsize + j];
		}
	}
}

void scroll_linescroll4(int num, Fixed32 scroll_val, int boundary1, int boundary2, int boundary3) {
	SclLineparam *lp = (num == SCROLL_BACKGROUND1 ? &line_param0 : &line_param1);

	for (int i = 0; i < 224; i++) {
		if (i < boundary1) {
			lp->line_tbl[i].h = (scroll_val >> 3);
		}
		else if (i < boundary2) {
			lp->line_tbl[i].h = (scroll_val >> 2);
		}
		else if (i < boundary3) {
			lp->line_tbl[i].h = (scroll_val >> 1);
		}
		else {
			lp->line_tbl[i].h = scroll_val;
		}
	}
	if (num == SCROLL_BACKGROUND1) {
		SCL_Open(SCL_NBG0);
	}
	else {
		SCL_Open(SCL_NBG1);
	}
	SCL_SetLineParam(lp);
	SCL_Close();
}

void scroll_changescreen(int direction) {
	int i, j, pos;
	Uint16 *lwram_ptr;
	Uint16 *tilemap_vram;

	if (direction == SCROLL_UP) {
		scrolls_y[SCROLL_PLAYFIELD] -= MTH_FIXED(224);
	}
	else {
		scrolls_y[SCROLL_PLAYFIELD] += MTH_FIXED(224);
	}
	map_tiles_y[SCROLL_PLAYFIELD] = scrolls_y[SCROLL_PLAYFIELD] >> 20;
	tilemap_vram = VRAM_PTR(2);
	lwram_ptr = (Uint16 *)LWRAM;

	for (i = 0; i < 14; i++) {
		for (j = -1; j < 31; j++) {
			pos = (i * 32) + ((j + map_tiles_x[SCROLL_PLAYFIELD]) % 32);
			if (pos >= 0) {
				tilemap_vram[pos] = scroll_get(SCROLL_PLAYFIELD, j + map_tiles_x[SCROLL_PLAYFIELD], i + map_tiles_y[SCROLL_PLAYFIELD]);
			}
		}
	}
}
