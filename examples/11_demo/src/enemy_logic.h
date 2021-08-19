#ifndef _ENEMY_LOGIC_H
#define _ENEMY_LOGIC_H
#include <stdint.h>

enum enemy_type
{
    ENEMY_STATIC = 0,
    ENEMY_MOVE,
    ENEMY_RANGE,
};

void update_enemy();

#endif