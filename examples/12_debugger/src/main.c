#include <stdint.h>
#include "ubox.h"

#define LOCAL
#include "tiles.h"

#define WHITESPACE_TILE 129


void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}

uint32_t sum(uint32_t a, uint32_t b)
{
    return a + b;
}

void echo_function(uint8_t a)
{
    uint8_t b = a;
}


void main()
{
    
    ubox_init_isr(2);

    ubox_set_mode(2);
  
    ubox_set_colors(1, 1, 1);

    ubox_disable_screen();

    ubox_set_tiles(tiles);
    ubox_set_tiles_colors(tiles_colors);
  
    ubox_fill_screen(WHITESPACE_TILE);
	
    ubox_enable_screen();

    uint8_t result = sum(5, 4);

    echo_function(result);

    while (1)
    {	
        ubox_wait();
    }
}
