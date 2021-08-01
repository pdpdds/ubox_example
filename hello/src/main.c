#include <stdint.h>
#include "ubox.h"


#define LOCAL
#include "main.h"

#define LOCAL
#include "tiles.h"
#undef LOCAL

void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}

void main()
{
    //  PAL: 50/2 = 25 FPS
    // NTSC: 60/2 = 30 FPS
    ubox_init_isr(2);

    // set screen 2
    ubox_set_mode(2);
    // all black
    ubox_set_colors(1, 1, 1);

    // avoid showing garbage on the screen
    // while setting up the tiles
    ubox_disable_screen();

   // upload our tileset
    ubox_set_tiles(tiles);
    // and the colour information
    ubox_set_tiles_colors(tiles_colors);

    // clear the screen
    ubox_fill_screen(WHITESPACE_TILE);
	put_text(11, 11, "HELLO WORLD!!");
		
    ubox_enable_screen();

    // reg 1: activate sprites, v-blank int on, 16x16 sprites
    //ubox_wvdp(1, 0xe2);


    // wait until fire is pressed
    // can be space (control will be cursors), or any fire button on a joystick
	
	
	
    while (1)
    {	
        ubox_wait();
    }
}
