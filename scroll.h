#ifndef SCROLL_H
#define SCROLL_H

#define TILE_SIZE 16
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define SCREEN_TILES_X 20
#define SCREEN_TILES_Y 14
#define CELL_SIZE 32

extern Sint32 map_tile_x, map_tile_y;

#define COPY_MODE_LCOL 1 
#define COPY_MODE_RCOL 2 
#define COPY_MODE_TROW 4 
#define COPY_MODE_BROW 8 

#define NBG0_MAP_ADDR (SCL_VDP2_VRAM_A0)
#define NBG1_MAP_ADDR (SCL_VDP2_VRAM_A0 + 0x800)

extern Uint32 copy_mode; //what to copy to VRAM from the map
extern Fixed32 scrolls_x[];
extern Fixed32 scrolls_y[];

void init_scroll(const Uint8 *tiles, const Uint16 *tilemap0, const Uint16 *tilemap1, const Uint32 *palette);
//translate scroll by x,y amounts
void move_scroll(int num, Fixed32 x, Fixed32 y);
//moves scroll absolutely to coordinates with no bounds checking
void set_scroll(int num, Fixed32 x, Fixed32 y);
//gets the value at the given coordinates for a square map
Uint16 get_map_val(int map, int x, int y);

#endif
