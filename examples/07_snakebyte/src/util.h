#ifndef _UTIL_H
#define _UTIL_H
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

void put_text(uint8_t x, uint8_t y, const uint8_t *text);
void RenderTile(int x, int y, int tileNum);
int now();
void my_isr();

#endif