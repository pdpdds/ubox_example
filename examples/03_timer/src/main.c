#include <stdint.h>
#include "ubox.h"
#include <stdio.h>

#define LOCAL
#include "tiles.h"

#define WHITESPACE_TILE 129

void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}


char buffer[10];

int g_count = 0;
int g_counter = 0;

void my_isr()
{
	g_counter++;	
}

void main()
{
    uint8_t wait_tick = 2;
    ubox_init_isr(wait_tick);

    ubox_set_mode(2);
  
    ubox_set_colors(1, 1, 1);

    ubox_disable_screen();

    ubox_set_tiles(tiles);
    ubox_set_tiles_colors(tiles_colors);
  
    ubox_fill_screen(WHITESPACE_TILE);
	put_text(11, 11, "COUNT : ");
	sprintf(buffer, "%d", g_count);
	put_text(11 + 9, 11, buffer);
		
    ubox_enable_screen();
	ubox_wvdp(1, 0xe2);

	ubox_set_user_isr(my_isr);

    while (1)
    {			
		
		if((g_counter / wait_tick) == 50)
		{
			g_count++;
			sprintf(buffer, "%d", g_count);
			put_text(11 + 9, 11, buffer);
			g_counter = 0;
		}
	
        ubox_wait();
		
    }
}
