#ifndef _UTIL_H
#define _UTIL_H
#include <stdint.h>

#define WHITESPACE_TILE  129
#define BLACK_TILE 	     85
#define GREEN_TILE 	     74
#define YELLOW_TILE      77
#define WHITE_TILE 	     81
#define RED_TILE 	     80

#define WHITESPACE_TILE 129

struct Rect
{
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
};

void InitEnvironment();
void RenderTile(uint8_t x, uint8_t y, uint8_t tileNum);
void RenderTiles(int x, int y, int x_count, int y_count, int tileNum);
void put_text(uint8_t x, uint8_t y, const uint8_t *text);
int now();
void my_isr();
int InRect(int posx, int posy, struct Rect *rect);

#endif