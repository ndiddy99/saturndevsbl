#include <string.h>
#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "scroll.h"

static Fixed32 scroll_x = 0;
static Fixed32 scroll_y = 0;
Sint32 map_tile_x = 0;
Sint32 map_tile_y = 0;
Uint32 copy_mode = 0;

/*----------------------------
SCROLL NBG0 Cycle Table
Pattern Name Table location:B0
Character location: B1
Color Mode: C256
Zoom Mode: 1
----------------------------*/
Uint16	CycleTb[]={
	0xffff,0xffff,
	0xffff,0xffff,
	0x0fff,0xffff,
	0x44ff,0xffff
};

void init_scroll(const Uint8 *tiles, const Uint16 *tilemap, const Uint32 *palette) {
	int count, i, j;
	Uint16 BackCol;
	Uint8 *VramWorkP;
	Uint16 *TilemapVram;
	SclConfig	scfg;
	SclVramConfig vram_cfg;
	
	SCL_Vdp2Init();
	
	SCL_SetColRamMode(SCL_CRM24_1024);
	SCL_AllocColRam(SCL_NBG0,256,OFF); //set up palette data
	SCL_SetColRam(SCL_NBG0,0,256,(void *)palette);
	BackCol = 0x0000; //set the background color to black
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B1; //scroll character pattern to VRAM B1
	memcpy(VramWorkP, tiles, 768);
	
	TilemapVram = (Uint16 *)SCL_VDP2_VRAM_B0;
	count = 0;
	for (i = 0; i < 32; i++) { //saturn tilemap is 32*32
		for (j = 0; j < 32; j++) {
			TilemapVram[count++] = tilemap[i * 64 + j]; //level is 64*64
			
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
		scfg.patnamecontrl = 0x000c; //vram B1 offset
		for(i=0;i<4;i++)   scfg.plate_addr[i] = SCL_VDP2_VRAM_B0;
	SCL_SetConfig(SCL_NBG0, &scfg);
	
	//setup VRAM configuration
	SCL_InitVramConfigTb(&vram_cfg);
		vram_cfg.vramModeA = ON; //separate VRAM A into A0 & A1
		vram_cfg.vramModeB = ON; //separate VRAM B into B0 & B1
	SCL_SetVramConfig(&vram_cfg);

	//setup vram access pattern
	SCL_SetCycleTable(CycleTb);
	
	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(0), FIXED(0),0); //home position
		SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();
}

void set_scroll(Uint32 num, Fixed32 x, Fixed32 y) {
	Sint32 curr_tile_x, curr_tile_y;
	scroll_x += x;
	scroll_y += y;
	if (scroll_x < 0) scroll_x = 0;
	if (scroll_x > FIXED((64 - SCREEN_TILES_X) * 16)) scroll_x = FIXED((64 - SCREEN_TILES_X) * 16);
	if (scroll_y < 0) scroll_y = 0;
	if (scroll_y > FIXED((64 - SCREEN_TILES_Y) * 16)) scroll_y = FIXED((64 - SCREEN_TILES_Y) * 16);
	curr_tile_x = MTH_FixedToInt(scroll_x) >> 4; //tile size is 16x16
	curr_tile_y = MTH_FixedToInt(scroll_y) >> 4;
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
	
	SCL_Open(num);
		SCL_MoveTo(scroll_x, scroll_y, 0);
	SCL_Close();
}

//gets the value at the given coordinates for a square map
Uint16 get_map_val(const Uint16 *map, int x, int y, int size) {
	if (x >= size || y >= size) {
		return 0;
	}
	else {
		return map[y * size + x];
	}
}
