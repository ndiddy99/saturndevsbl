#ifndef SCROLL_H
#define SCROLL_H

#define TILE_SIZE 16
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
//maximum # tiles i'll show onscreen
#define SCREEN_TILES_X 28
#define SCREEN_TILES_Y 20
#define BG_TILES 32

extern Sint32 map_tiles_x[], map_tiles_y[];

#define COPY_MODE_LCOL 1 
#define COPY_MODE_RCOL 2 
#define COPY_MODE_TROW 4 
#define COPY_MODE_BROW 8 

#define NBG0_MAP_ADDR (SCL_VDP2_VRAM_A0)
#define NBG1_MAP_ADDR (SCL_VDP2_VRAM_A0 + 0x800)

extern Uint32 copy_modes[]; //what to copy to VRAM from the map
extern Fixed32 scrolls_x[];
extern Fixed32 scrolls_y[];

void scroll_init(const Uint8 *tiles, const Uint16 *tilemap0, const Uint16 *tilemap1, const Uint32 *palette);
//translate scroll by x,y amounts
void scroll_move(int num, Fixed32 x, Fixed32 y);
//moves scroll absolutely to coordinates
void scroll_set(int num, Fixed32 x, Fixed32 y);
//sets the scale of a bg
void scroll_scale(int num, Fixed32 scale);
//gets the value at the given coordinates for a square map
Uint16 scroll_get(int map, int x, int y);
//copies scroll to VRAM after position has been changed by move/set scroll
void scroll_copy(int num);
#endif
