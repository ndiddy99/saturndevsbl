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

#define SCROLL_PLAYFIELD (0)
#define SCROLL_FOREGROUND (1)
#define SCROLL_BACKGROUND1 (2)
#define SCROLL_BACKGROUND2 (3)

#define SCROLL_HMIRROR (0x400)
#define SCROLL_VMIRROR (0x800)

extern Uint32 copy_modes[]; //what to copy to VRAM from the map
extern Fixed32 scrolls_x[];
extern Fixed32 scrolls_y[];


typedef struct {
    Fixed32 player_startx; //where the player starts in the level
    Fixed32 player_starty;

    Uint8 *playfield_tile_filename;
    Uint16 playfield_tile_num; //number of tiles in the playfield
    Uint32 *playfield_palette; //256 color playfield palette
    Uint8 *playfield_map_filename;
    Uint16 playfield_map_width;
    Uint16 playfield_map_height;
} LEVEL;

enum transition_states {
    TSTATE_NULL = 0,
    TSTATE_PRESETUP,
    TSTATE_ZOOM,
    TSTATE_POSTSETUP
};
extern int scroll_transition_state;

void scroll_init(LEVEL *level);
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
//sets all backgrounds to how they were on init
void scroll_reset(void);
#endif
