#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "ubox.h"
#include <assert.h>

#define LOCAL
#include "tiles.h"

#define WHITESPACE_TILE 129


void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}

void HALT()
{
    while(1) {
        __asm__ ("halt"); 
    };
}

void UBOX_FATAL(const char* message, int line_number)
{
    char fileName[256];
    char error_message[256];

    strcpy(fileName, __FILE__);
    for(int i = 0; i < strlen(fileName); i++)
    {
        fileName[i] = (char)toupper((fileName[i]));
    }

    sprintf(error_message, "%s %d %s", fileName, line_number, message);
    put_text(0, 0, error_message);
    //HALT();
}

void main()
{
    
    ubox_init_isr(2);

    ubox_set_mode(2);
  
    ubox_set_colors(1, 1, 1);

    ubox_disable_screen();

    ubox_set_tiles(tiles);
    ubox_set_tiles_colors(tiles_colors);

    ubox_enable_screen();
  
    ubox_fill_screen(WHITESPACE_TILE);
	
    char* data = malloc(1000000);

    if(data == 0)
    {  
        UBOX_FATAL("DATA IS NULL", __LINE__);
        UBOX_FATAL("NOT PRINTED!!", __LINE__);
    }

    while (1)
    {	
        ubox_wait();
    }
}
