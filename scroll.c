#include <string.h>
#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "scroll.h"

Fixed32 scrolls_x[] = {0, 0, 0, 0};
Fixed32 scrolls_y[] = {0, 0, 0, 0};
Sint32 map_tiles_x[] = {0, 0, 0, 0};
Sint32 map_tiles_y[] = {0, 0, 0, 0};
Uint32 copy_modes[] = {0, 0, 0, 0};
Uint16 *maps[4];
Uint16 *vram[] = {(Uint16 *)NBG0_MAP_ADDR, (Uint16 *)NBG1_MAP_ADDR, NULL, NULL};

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

// There's also numerous read restrictions, see SOA technical bulletin #8 for more information

Uint16	CycleTb[]={
	0x011f,0xffff,
	0x4455,0xff55,
	0xffff,0xffff,
	0xffff,0xffff
};

void scroll_init(const Uint8 *tiles, const Uint16 *tilemap0, const Uint16 *tilemap1, const Uint32 *palette) {
	int count, i, j;
	Uint16 BackCol;
	Uint8 *VramWorkP;
	Uint16 *TilemapVram;
	SclConfig	scfg;
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

	
	TilemapVram = (Uint16 *)NBG0_MAP_ADDR;
	count = 0;
	for (i = 0; i < 32; i++) { //saturn tilemap is 32*32
		for (j = 0; j < 32; j++) {
			TilemapVram[count++] = tilemap0[i * 64 + j]; //level is 64*64
			
		}
	}

	TilemapVram = (Uint16 *)NBG1_MAP_ADDR;
	count = 0;
	for (i = 0; i < 32; i++) { //saturn tilemap is 32*32
		for (j = 0; j < 32; j++) {
			TilemapVram[count++] = tilemap1[i * 64 + j]; //level is 64*64
			
		}
	}
	//scroll initial configuration
	SCL_InitConfigTb(&scfg);
		scfg.dispenbl      = ON;
		scfg.charsize      = SCL_CHAR_SIZE_2X2;
		scfg.pnamesize     = SCL_PN1WORD;
		scfg.flip          = SCL_PN_10BIT;
		scfg.platesize     = SCL_PL_SIZE_1X1; //they meant "plane size"
		scfg.coltype       = SCL_COL_TYPE_256;
		scfg.datatype      = SCL_CELL;
		scfg.patnamecontrl = 0x0004; //vram A1 offset
		for(i=0;i<4;i++)   scfg.plate_addr[i] = NBG0_MAP_ADDR;
	SCL_SetConfig(SCL_NBG0, &scfg);
		for(i=0;i<4;i++)   scfg.plate_addr[i] = NBG1_MAP_ADDR;
	SCL_SetConfig(SCL_NBG1, &scfg);
	
	//setup VRAM configuration
	SCL_InitVramConfigTb(&vram_cfg);
		vram_cfg.vramModeA = ON; //separate VRAM A into A0 & A1
		vram_cfg.vramModeB = ON; //separate VRAM B into B0 & B1
	SCL_SetVramConfig(&vram_cfg);

	//setup vram access pattern
	SCL_SetCycleTable(CycleTb);
	 
	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(48), FIXED(10),0); //home position
		SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();
	SCL_Open(SCL_NBG1);
		SCL_MoveTo(FIXED(0), FIXED(0), 0);
		SCL_Scale(FIXED(0.75), FIXED(0.75));
	SCL_Close();
	
	maps[0] = (Uint16 *)tilemap0;
	maps[1] = (Uint16 *)tilemap1;

}

void scroll_move(int num, Fixed32 x, Fixed32 y) {
	Sint32 curr_tile_x, curr_tile_y;
	scrolls_x[num] += x;
	scrolls_y[num] += y;
	curr_tile_x = MTH_FixedToInt(scrolls_x[num]) >> 4; //tile size is 16x16
	curr_tile_y = MTH_FixedToInt(scrolls_y[num]) >> 4;
	copy_modes[num] = 0;
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

void scroll_scale(int num, Fixed32 scale) {
	SCL_Open(1 << (num + 2));
		SCL_Scale(scale, scale);
	SCL_Close();
}

//gets the value at the given coordinates for a square map
Uint16 scroll_get(int map, int x, int y) {
	Uint16 *map_ptr = maps[map];
	if (x >= 64 || y >= 64) {
		return 0;
	}
	return map_ptr[y * 64 + x];
}

void scroll_copy(int num) {
	int i;
	Uint16 *vram_ptr = vram[num];
	if (copy_modes[num] & COPY_MODE_RCOL) {
		for (i = -1; i < SCREEN_TILES_Y + 1; i++) {
			vram_ptr[(((i + map_tiles_y[num]) % 32) * 32) + ((map_tiles_x[num] + SCREEN_TILES_X) % 32)] = 
				scroll_get(num, map_tiles_x[num] + SCREEN_TILES_X, map_tiles_y[num] + i);
		}
	}
	if (copy_modes[num] & COPY_MODE_LCOL) {
		for (i = -1; i < SCREEN_TILES_Y + 1; i++) {
			vram_ptr[(((i + map_tiles_y[num]) % 32) * 32) + ((map_tiles_x[num] - 1) % 32)] = 
				scroll_get(num, map_tiles_x[num] - 1, map_tiles_y[num] + i);
		}		
	}
	if (copy_modes[num] & COPY_MODE_BROW) {
		for (i = -1; i < SCREEN_TILES_X + 1; i++) {
			vram_ptr[(((map_tiles_y[num] + SCREEN_TILES_Y) % 32) * 32) + ((i + map_tiles_x[num]) % 32)] =
				scroll_get(num, map_tiles_x[num] + i, map_tiles_y[num] + SCREEN_TILES_Y);
		}
	}
	if (copy_modes[num] & COPY_MODE_TROW) {
		for (i = -1; i < SCREEN_TILES_X + 1; i++) {
			vram_ptr[(((map_tiles_y[num] - 1) % 32) * 32) + ((i + map_tiles_x[num]) % 32)] =
				scroll_get(num, map_tiles_x[num] + i, map_tiles_y[num] - 1);
		}
	}
}
