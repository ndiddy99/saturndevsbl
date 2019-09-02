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

//cloud.c
extern Uint32 cloud_pal[];
extern Uint8 cloud_chr[];

//cloud_map.c
extern Uint16 map_cloud[];

#define TEST_PAL_SIZE 256
#define TEST_CHR_SIZE (64 * 64)

#endif
