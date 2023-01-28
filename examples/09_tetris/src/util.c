#include "util.h"
#include "ubox.h"

#define LOCAL
#include "tiles.h"

volatile long g_tick_count = 0; //틱 카운트

void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}

void RenderTile(uint8_t x, uint8_t y, uint8_t tileNum)
{

    ubox_put_tile(x, y, tileNum);
}

void RenderTiles(int x, int y, int x_count, int y_count, int tileNum)
{
   
    for(int i = 0; i < y_count; i++)
        for(int j = 0; j < x_count; j++)
            ubox_put_tile(x + j, y + i, tileNum);

}

void InitEnvironment()
{
    ubox_init_isr(2);

    ubox_set_mode(2);
    ubox_set_colors(1, 1, 1);

    ubox_disable_screen();

    ubox_set_tiles(tiles);
    ubox_set_tiles_colors(tiles_colors);

    ubox_fill_screen(WHITESPACE_TILE);

    ubox_enable_screen();

	ubox_set_user_isr(my_isr);
    ubox_wvdp(1, 0xe2);
}

int now()
{
    return g_tick_count;
}

void my_isr()
{
    g_tick_count++;
}

int InRect(int posx, int posy, struct Rect *rect)
{
	if (posx >= rect->x1 && posx < rect->x2 && posy >= rect->y1 && posy < rect->y2)
		return 1;

	return 0;
}
