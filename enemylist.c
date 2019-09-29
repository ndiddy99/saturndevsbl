#include <sega_mth.h>
#include "circle.h"
#include "enemylist.h"

#define TYPE_CIRCLE (1)
#define END_LIST (0)

//list format: type, x position (tiles), y position (tiles)
void (*enemy_arr[])(Fixed32, Fixed32) = {NULL, circle_make};
Uint16 floor0_enemies[] = {TYPE_CIRCLE, 43, 40, END_LIST};
Uint16 floor1_enemies[] = {TYPE_CIRCLE, 6, 4, TYPE_CIRCLE, 14, 11, END_LIST};
Uint16 floor2_enemies[] = {TYPE_CIRCLE, 13, 21, TYPE_CIRCLE, 10, 3, END_LIST};
Uint16 *enemy_lists[] = {floor0_enemies, floor1_enemies, floor2_enemies};

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
