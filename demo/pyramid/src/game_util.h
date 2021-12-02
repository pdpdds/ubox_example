#ifndef _GAME_UTIL_H
#define _GAME_UTIL_H
#include <stdint.h>

uint8_t is_map_blocked(uint8_t x, uint8_t y);
uint8_t is_map_jewel(uint8_t x, uint8_t y);
uint8_t check_floor(uint8_t x, uint8_t y);
struct entity *find_object(uint8_t id);
struct entity* check_foothold(uint8_t x, uint8_t y);
extern struct entity *find_collide_object(uint8_t x, uint8_t y, int type);

#endif