#ifndef GRAPHICREFS_H
#define GRAPHICREFS_H

//sprite.c
extern Uint16 dimensions[];
extern Uint16 *tiles[];

//wood.c
extern Uint32 wood_pal[];
extern Uint8 wood_chr[];

//map1.c
extern Uint16 map1[];
//map2.c
extern Uint16 map2[];
//map3.c
extern Uint16 map3[];

//bg0.c
extern Uint32 bg0_pal[];
extern Uint8 bg0_chr[];

//bg0_2map.c
extern Uint16 bg0_2map[];

//bg0_3map.c
extern Uint16 bg0_3map[];

#define TEST_PAL_SIZE 256
#define TEST_CHR_SIZE (64 * 64)

#endif
