#ifndef _UTIL_H
#define _UTIL_H
#include <stdint.h>

#define WHITESPACE_TILE 129

struct Rect
{
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
};

void InitEnvironnmet();
void RenderTile(int x, int y, int tileNum);
void RenderTiles(int x, int y, int x_count, int y_count, int tileNum);
void put_text(uint8_t x, uint8_t y, const uint8_t *text);
int now();
void my_isr();
int InRect(int posx, int posy, struct Rect *rect);
void EraseTile(int x, int y, int x_count, int y_count, int tileNum);

#endif