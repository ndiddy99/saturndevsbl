#include <string.h>
#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "graphicrefs.h"
#include "player.h"
#include "scroll.h"

Fixed32 scrolls_x[] = {0, 0, 0, 0};
Fixed32 scrolls_y[] = {0, 0, 0, 0};
Sint32 map_tiles_x[] = {0, 0, 0, 0};
Sint32 map_tiles_y[] = {0, 0, 0, 0};
Uint32 copy_modes[] = {0, 0, 0, 0};
Uint16 *maps[4]; //map locations in WRAM for the 4 backgrounds
Uint16 *tilemaps[] = {map1, map2, map3}; //all the maps currently in WRAM
int curr_map = 0; //which map the player is currently on
Uint32 vram[] = {SCL_VDP2_VRAM_A0, SCL_VDP2_VRAM_A0 + 0x800, NULL, NULL}; //where in VRAM each tilemap is
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
	0x011f,0xffff,
	0x5555,0xff44,
	0xffff,0xffff,
	0xffff,0xffff
};

SclConfig scfg0;
SclConfig scfg1;

void scroll_init(const Uint8 *tiles, const Uint16 *tilemap0, const Uint16 *tilemap1, const Uint32 *palette) {
	int count, i, j;
	Uint16 BackCol;
	Uint8 *VramWorkP;
	Uint16 *TilemapVram;
	SclVramConfig vram_cfg;

	SCL_SetColRamMode(SCL_CRM24_1024);
		SCL_AllocColRam(SCL_NBG0,256,OFF); //set up palette data
		SCL_SetColRam(SCL_NBG0,0,256,(void *)palette);
		SCL_AllocColRam(SCL_NBG1, 256, OFF);
		SCL_SetColRam(SCL_NBG1, 0, 256, (void *)palette);
		BackCol = 0x0000; //set the background color to black
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);
	
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_A1; //scroll character pattern to VRAM A1
	memcpy(VramWorkP, tiles, 256 * 40);

	
	TilemapVram = VRAM_PTR(0);
	count = 0;
	for (i = 0; i < 32; i++) { //saturn tilemap is 32*32
		for (j = 0; j < 32; j++) {
			TilemapVram[count++] = tilemap0[i * 64 + j]; //level is 64*64
			
		}
	}

	TilemapVram = VRAM_PTR(1);
	count = 0;
	for (i = 0; i < 32; i++) { //saturn tilemap is 32*32
		for (j = 0; j < 32; j++) {
			TilemapVram[count++] = tilemap1[i * 64 + j]; //level is 64*64
			
		}
	}
	//scroll initial configuration
	SCL_InitConfigTb(&scfg0);
		scfg0.dispenbl      = ON;
		scfg0.charsize      = SCL_CHAR_SIZE_2X2;
		scfg0.pnamesize     = SCL_PN1WORD;
		scfg0.flip          = SCL_PN_10BIT;
		scfg0.platesize     = SCL_PL_SIZE_1X1; //they meant "plane size"
		scfg0.coltype       = SCL_COL_TYPE_256;
		scfg0.datatype      = SCL_CELL;
		scfg0.patnamecontrl = 0x0004; //vram A1 offset
	for(i=0;i<4;i++)   scfg0.plate_addr[i] = vram[0];
	SCL_SetConfig(SCL_NBG0, &scfg0);
	memcpy((void *)&scfg1, (void *)&scfg0, sizeof(SclConfig));
	for(i=0;i<4;i++)   scfg1.plate_addr[i] = vram[1];
	SCL_SetConfig(SCL_NBG1, &scfg1);
	
	//setup VRAM configuration
	SCL_InitVramConfigTb(&vram_cfg);
		vram_cfg.vramModeA = ON; //separate VRAM A into A0 & A1
		vram_cfg.vramModeB = ON; //separate VRAM B into B0 & B1
	SCL_SetVramConfig(&vram_cfg);

	//setup vram access pattern
	SCL_SetCycleTable(CycleTb);
	 
	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(48), FIXED(10),0); //home position
	SCL_Close();
	SCL_Open(SCL_NBG1);
		SCL_MoveTo(FIXED(0), FIXED(0), 0);
	SCL_Close();
	scroll_scale(0, FIXED(1));
	scroll_scale(1, FIXED(0.75));

	maps[0] = (Uint16 *)tilemap0;
	maps[1] = (Uint16 *)tilemap1;

}

void scroll_move(int num, Fixed32 x, Fixed32 y) {
	Sint32 curr_tile_x, curr_tile_y;
	scrolls_x[num] += x;
	scrolls_y[num] += y;
	curr_tile_x = MTH_FixedToInt(scrolls_x[num]) >> 4; //tile size is 16x16
	curr_tile_y = MTH_FixedToInt(scrolls_y[num]) >> 4;
	// copy_modes[num] = 0;
	if (curr_tile_x - map_tiles_x[num] > 0) { //if x value increasing
		copy_modes[num] |= COPY_MODE_RCOL;
	}
	else if (curr_tile_x - map_tiles_x[num] < 0) { //if x value decreasing
		copy_modes[num] |= COPY_MODE_LCOL;
	}
	if (curr_tile_y - map_tiles_y[num] > 0) { //if y value increasing
		copy_modes[num] |= COPY_MODE_BROW;
	}
	else if (curr_tile_y - map_tiles_y[num] < 0) { //if y value decreasing
		copy_modes[num] |= COPY_MODE_TROW;
	}
	map_tiles_x[num] = curr_tile_x;
	map_tiles_y[num] = curr_tile_y;
	
	//Scroll bitmasks are:
	//NBG0 - (1 << 2)
	//NBG1 - (1 << 3)
	//etc
	SCL_Open(1 << (num + 2));
		SCL_MoveTo(scrolls_x[num], scrolls_y[num], 0);
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
		return;
	}
	else if (scale < FIXED(1) && scale >= FIXED(0.5)) {
		Scl_n_reg.zoomenbl |= (num == 0 ? ZOOM_HALF_NBG0 : ZOOM_HALF_NBG1);
	}
	else {
		Scl_n_reg.zoomenbl |= (num == 0 ? ZOOM_QUARTER_NBG0 : ZOOM_QUARTER_NBG1);
	}
}

//gets the value at the given coordinates for a square map
Uint16 scroll_get(int map, int x, int y) {
	Uint16 *map_ptr = maps[map];
	if (x >= 64 || x < 0 || y >= 64 || y < 0) {
		return 0;
	}
	return map_ptr[y * 64 + x];
}

void scroll_copy(int num) {
	int i, pos;
	
	Uint16 *vram_ptr = VRAM_PTR(num);
	if (copy_modes[num] & COPY_MODE_RCOL) {
		for (i = -1; i < SCREEN_TILES_Y + 1; i++) {
			pos = (((i + map_tiles_y[num]) % 32) * 32) + ((map_tiles_x[num] + SCREEN_TILES_X) % 32);
			if (pos >= 0) {
				vram_ptr[pos] = scroll_get(num, map_tiles_x[num] + SCREEN_TILES_X, map_tiles_y[num] + i);
			}
		}
	}
	if (copy_modes[num] & COPY_MODE_LCOL) {
		for (i = -1; i < SCREEN_TILES_Y + 1; i++) {
			pos = (((i + map_tiles_y[num]) % 32) * 32) + ((map_tiles_x[num] - 1) % 32);
			if (pos >= 0) {
				vram_ptr[pos] = scroll_get(num, map_tiles_x[num] - 1, map_tiles_y[num] + i);
			}
		}		
	}
	if (copy_modes[num] & COPY_MODE_BROW) {
		for (i = -1; i < SCREEN_TILES_X + 1; i++) {
			pos = (((map_tiles_y[num] + SCREEN_TILES_Y) % 32) * 32) + ((i + map_tiles_x[num]) % 32);
			if (pos >= 0) {
				vram_ptr[pos] = scroll_get(num, map_tiles_x[num] + i, map_tiles_y[num] + SCREEN_TILES_Y);
			}
		}
	}
	if (copy_modes[num] & COPY_MODE_TROW) {
		for (i = -1; i < SCREEN_TILES_X + 1; i++) {
			pos = (((map_tiles_y[num] - 1) % 32) * 32) + ((i + map_tiles_x[num]) % 32);
			if (pos >= 0) {
				vram_ptr[(((map_tiles_y[num] - 1) % 32) * 32) + ((i + map_tiles_x[num]) % 32)] =
					scroll_get(num, map_tiles_x[num] + i, map_tiles_y[num] - 1);
			}
		}
	}
}

void scroll_transition() {
	static Fixed32 scale_val, initial_x, initial_y;
	static int count;
	Uint16 *TilemapVram;
	Uint16 *TilemapWram;
	int i, j;
	Uint32 temp;

	switch (scroll_transition_state) {
		case TSTATE_PRESETUP:
			TilemapVram = VRAM_PTR(0);
			curr_map++;
			TilemapWram = tilemaps[curr_map + 1];
			scfg0.dispenbl = OFF;
			SCL_SetConfig(SCL_NBG0, &scfg0); //disable NBG0
			SCL_DisplayFrame();
			count = 0;
			for (i = 0; i < 32; i++) { //saturn tilemap is 32*32
				for (j = 0; j < 32; j++) {
					TilemapVram[count++] = TilemapWram[i * 64 + j]; //level is 64*64
				}
			}
			scale_val = FIXED(0.75);
			initial_x = scrolls_x[1];
			initial_y = scrolls_y[1];
			count = 0;
			scroll_transition_state = TSTATE_ZOOM;
		break;
		case TSTATE_ZOOM:
			scroll_scale(1, scale_val);
			scroll_move(1, FIXED(1.33), FIXED(1.33));
			scale_val += FIXED(0.01);
			if (scale_val >= FIXED(1)) {
				scroll_scale(1, FIXED(1));
				scroll_transition_state = TSTATE_POSTSETUP;
			}
		break;
		case TSTATE_POSTSETUP:
			temp = vram[0]; //swap VRAM addresses
			vram[0] = vram[1];
			vram[1] = temp;
			for (i = 0; i < 4; i++) scfg0.plate_addr[i] = vram[0];
			for (i = 0; i < 4; i++) scfg1.plate_addr[i] = vram[1];
			scfg0.dispenbl = ON;
			scfg1.dispenbl = ON;
			SCL_SetConfig(SCL_NBG0, &scfg0);
			SCL_SetConfig(SCL_NBG1, &scfg1);
			scroll_scale(1, FIXED(0.75));
			maps[0] = tilemaps[curr_map];
			maps[1] = tilemaps[curr_map + 1];
			player.xPos = scrolls_x[1] + PLAYER_SPRITE_X;
			player.yPos = scrolls_y[1] + PLAYER_SPRITE_Y;
			scroll_transition_state = TSTATE_NULL;
		break;
	}
}
