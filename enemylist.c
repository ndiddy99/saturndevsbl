#include <sega_mth.h>
#include "enemylist.h"
#include "spritecode/float.h"

#define TYPE_FLOAT (1)
#define END_LIST (0)

//list format: type, x position (tiles), y position (tiles)
void (*enemy_arr[])(Fixed32, Fixed32) = {NULL, float_make};
Uint16 level0_enemies[] = {TYPE_FLOAT, 10, 8, END_LIST};
Uint16 *enemy_lists[] = {level0_enemies};

void enemylist_spawn(int floor) {
    int index = 0;
    Uint16 *enemy_list = enemy_lists[floor];
    while (1) {
        if (enemy_list[index] == END_LIST) {
            return;
        }
        //convert from where sprite is on the tilemap to where it is in pixels
        Fixed32 x = MTH_IntToFixed(enemy_list[index + 1] << 4);
        Fixed32 y = MTH_IntToFixed(enemy_list[index + 2] << 4);
        (*enemy_arr[enemy_list[index]])(x, y);
        index += 3;
    }
}
