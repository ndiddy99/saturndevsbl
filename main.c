#include	<string.h> //memcpy
#include	<machine.h>
#define		_SPR2_
#include	<sega_spr.h>
#include	<sega_scl.h> 
#include	<sega_mth.h>

#include	"v_blank\v_blank.h"
#include	"sprite.h"

#include	"graphic\cosmo2u.cha"
#include	"graphic\cosmo2u.col"
#include	"graphic\cosmo2u.map"
#include	"graphic\face.c"

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

#define CommandMax    300
#define GourTblMax    300
#define LookupTblMax  100
#define CharMax       16
#define DrawPrtyMax   256
SPR_2DefineWork(work2D, CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)


int main()
{
	SclConfig	scfg;
	SclRgb		start,end;
	Uint16  	BackCol,i,PadData1EW;
	Uint8   	*VramWorkP;
	SPRITE_INFO sprite;

	SCL_Vdp2Init();
	SPR_2Initial(&work2D);
	SCL_SetColRamMode(SCL_CRM24_1024);
	
	SetVblank(); //setup vblank routine
	set_imask(0);
	
	SPR_2FrameChgIntr(1); //wait until next frame to set color mode
	SCL_DisplayFrame();
	SCL_DisplayFrame();

	BackCol = 0x0000; //set the background color to black
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B1; //scroll character pattern to VRAM B1
	memcpy(VramWorkP, cosmo2u_char, sizeof(cosmo2u_char));

	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B0; //setup scroll pattern name table
	for(i=0;i<28;i++) {
		memcpy(VramWorkP, &cosmo2u_map[i*40],40*2);
		VramWorkP += 64*2;
	}

	SCL_AllocColRam(SCL_NBG0,256,OFF); //set up palette data
	SCL_SetColRam(SCL_NBG0,0,sizeof(cosmo2u_col),(void *)cosmo2u_col);

	//scroll initial configuration
	SCL_InitConfigTb(&scfg);
		scfg.dispenbl      = ON;
		scfg.charsize      = SCL_CHAR_SIZE_1X1;
		scfg.pnamesize     = SCL_PN1WORD;
		scfg.flip          = SCL_PN_12BIT;
		scfg.platesize     = SCL_PL_SIZE_2X2;
		scfg.coltype       = SCL_COL_TYPE_256;
		scfg.datatype      = SCL_CELL;
		scfg.patnamecontrl = 0x000c;// VRAM B1 offset
		for(i=0;i<4;i++)   scfg.plate_addr[i] = SCL_VDP2_VRAM_B0;
	SCL_SetConfig(SCL_NBG0, &scfg);

	//setup vram access pattern
	SCL_SetCycleTable(CycleTb);

	SCL_SetPriority(SCL_NBG0,7); //set layer priorities
	SCL_SetPriority(SCL_SPR,7);
	SCL_SetSpriteMode(SCL_TYPE5,SCL_MIX,SCL_SP_WINDOW);
	SPR_2SetChar(0, COLOR_5, 0, 64, 51, (char *)&bunny);
	
	sprite.charNum = 0;
	sprite.xPos = MTH_FIXED(20);
	sprite.yPos = MTH_FIXED(20);
	sprite.mirror = 0;
	sprite.xSize = 64;
	sprite.ySize = 51;
	sprite.scale = MTH_FIXED(1);
	sprite.angle = 0;

	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(0), FIXED(0),0); //home position
		SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();

	start.red = start.green = start.blue = 0;
	end.red = end.green = end.blue = - 255;
	while(1) {
		PadData1EW = PadData1E;
		PadData1E = 0;
		sprite.mirror = 0;
		
		if(PadData1 & PAD_U){
			// SCL_SetColOffset(SCL_OFFSET_A,SCL_NBG0,start.red,start.green,start.blue);
			// SCL_SetAutoColOffset(SCL_OFFSET_A,1,10,&start,&end);
			sprite.yPos -= MTH_FIXED(1);
		}
		if(PadData1 & PAD_D){
			// SCL_SetColOffset(SCL_OFFSET_A,SCL_NBG0,end.red,end.green,end.blue);
			// SCL_SetAutoColOffset(SCL_OFFSET_A,1,10,&end,&start);
			sprite.yPos += MTH_FIXED(1);
		}
		if (PadData1 & PAD_L) sprite.xPos -= MTH_FIXED(1);
		if (PadData1 & PAD_R) sprite.xPos += MTH_FIXED(1);
		if((PadData1 & PAD_X)) {
			sprite.angle += MTH_FIXED(3);
			if (sprite.angle > MTH_FIXED(180))
				sprite.angle -= MTH_FIXED(360);
		}
		if((PadData1 & PAD_Y)) {
			sprite.angle -= MTH_FIXED(3);
			if (sprite.angle < MTH_FIXED(-180))
				sprite.angle += MTH_FIXED(360);
		}
		if (PadData1 & PAD_Z)	sprite.mirror |= MIRROR_VERT; 
		if((PadData1 & PAD_A)) 	sprite.scale += MTH_FIXED(0.05);
		if((PadData1 & PAD_B)) 	sprite.scale -= MTH_FIXED(0.05);
		if (PadData1 & PAD_C)	sprite.mirror |= MIRROR_HORIZ;
		
		SPR_2OpenCommand(SPR_2DRAW_PRTY_OFF);
			drawSprite(&sprite);
		SPR_2CloseCommand();
		
		SCL_DisplayFrame();
	}
	return 0;
}
