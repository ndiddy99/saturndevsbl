#include <string.h>
#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "scroll.h"

Fixed32 scrolls_x[] = {0, 0, 0, 0};
Fixed32 scrolls_y[] = {0, 0, 0, 0};
Sint32 map_tile_x = 0;
Sint32 map_tile_y = 0;
Uint32 copy_mode = 0;
Uint16 *maps[3];

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

void init_scroll(const Uint8 *tiles, const Uint16 *tilemap0, const Uint16 *tilemap1, const Uint32 *palette) {
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
		SCL_Scale(FIXED(0.7), FIXED(0.7));
	SCL_Close();
	
	maps[0] = (Uint16 *)tilemap0;
	maps[1] = (Uint16 *)tilemap1;

}

void move_scroll(int num, Fixed32 x, Fixed32 y) {
	Sint32 curr_tile_x, curr_tile_y;
	Fixed32 *scroll_x = &scrolls_x[num];
	Fixed32 *scroll_y = &scrolls_y[num];
	*scroll_x += x;
	*scroll_y += y;
	if (*scroll_x < 0) *scroll_x = 0;
	if (*scroll_x > FIXED((64 - SCREEN_TILES_X) * 16)) *scroll_x = FIXED((64 - SCREEN_TILES_X) * 16);
	if (*scroll_y < 0) *scroll_y = 0;
	if (*scroll_y > FIXED((64 - SCREEN_TILES_Y) * 16)) *scroll_y = FIXED((64 - SCREEN_TILES_Y) * 16);
	curr_tile_x = MTH_FixedToInt(*scroll_x) >> 4; //tile size is 16x16
	curr_tile_y = MTH_FixedToInt(*scroll_y) >> 4;
	copy_mode = 0;
	if (curr_tile_x - map_tile_x > 0) { //if x value increasing
		copy_mode |= COPY_MODE_RCOL;
	}
	else if (map_tile_x - curr_tile_x > 0) { //if x value decreasing
		copy_mode |= COPY_MODE_LCOL;
	}
	if (curr_tile_y - map_tile_y > 0) { //if y value increasing
		copy_mode |= COPY_MODE_BROW;
	}
	else if (map_tile_y - curr_tile_y > 0) { //if y value decreasing
		copy_mode |= COPY_MODE_TROW;
	}
	map_tile_x = curr_tile_x;
	map_tile_y = curr_tile_y;
	
	//Scroll bitmasks are:
	//NBG0 - (1 << 2)
	//NBG1 - (1 << 3)
	//etc
	SCL_Open(1 << (num + 2));
		SCL_MoveTo(*scroll_x, *scroll_y, 0);
	SCL_Close();
}

//moves scroll absolutely to coordinates
void set_scroll(int num, Fixed32 x, Fixed32 y) {
	move_scroll(num, x - scrolls_x[num], y - scrolls_y[num]);
}

//gets the value at the given coordinates for a square map
Uint16 get_map_val(int map, int x, int y) {
	Uint16 *map_ptr = maps[map];
	if (x >= 64 || y >= 64) {
		return 0;
	}
	return map_ptr[y * 64 + x];
}
