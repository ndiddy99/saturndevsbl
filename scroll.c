#include <string.h>
#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h> 

static Fixed32 scroll_x;
static Fixed32 scroll_y;

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
	0x0f44,0xffff,
	0xffff,0xffff
};

void init_scroll(const Uint8 *tiles, const Uint16 *tilemap, const Uint32 *palette) {
	int i;
	Uint16 BackCol;
	Uint8 *VramWorkP;
	SclConfig	scfg;
	
	SCL_Vdp2Init();
	SCL_SetColRamMode(SCL_CRM24_1024);
	
	BackCol = 0x0000; //set the background color to black
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B1; //scroll character pattern to VRAM B1
	memcpy(VramWorkP, tiles, 768);

	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B0; //setup scroll pattern name table
	memcpy(VramWorkP, tilemap, 40 * 28);
	SCL_AllocColRam(SCL_NBG0,256,OFF); //set up palette data
	SCL_SetColRam(SCL_NBG0,0,256,(void *)palette);

	//scroll initial configuration
	SCL_InitConfigTb(&scfg);
		scfg.dispenbl      = ON;
		scfg.charsize      = SCL_CHAR_SIZE_2X2;
		scfg.pnamesize     = SCL_PN1WORD;
		scfg.flip          = SCL_PN_10BIT;
		scfg.platesize     = SCL_PL_SIZE_1X1;
		scfg.coltype       = SCL_COL_TYPE_256;
		scfg.datatype      = SCL_CELL;
		scfg.patnamecontrl = 0x000c; //vram B1 offset
		for(i=0;i<4;i++)   scfg.plate_addr[i] = SCL_VDP2_VRAM_B0;
	SCL_SetConfig(SCL_NBG0, &scfg);

	//setup vram access pattern
	SCL_SetCycleTable(CycleTb);
	
	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(0), FIXED(0),0); //home position
		SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();
}
