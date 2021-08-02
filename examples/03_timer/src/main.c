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

void _putchar(char character)
{
	ubox_put_tile(0, 0, character + 128 - 31);	
}

char buffer[10];

int count = 0;
int tick = 0;

void my_isr()
{
     // count interrupts
	 
	 
     ++tick;
	 
	 if(tick % 30 == 0)
	 {		 		 
		 count++;
	 }
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
	//put_text(11, 11, "HELLO WORLD!!");
		
    ubox_enable_screen();
	
	ubox_set_user_isr(my_isr);

	
    while (1)
    {			
		sprintf(buffer, "COUNT : %d", count);
		put_text(11, 11, buffer);
	
        ubox_wait();
		
    }
}
