#include <stdint.h>
#include "ubox.h"

#define MAP_W 32
#define MAP_H 21
#define WHITESPACE_TILE 129

uint8_t cur_map_data[MAP_W * MAP_H];

void draw_map()
{
    for (int i = 0; i < MAP_W; i++)
    {
        cur_map_data[i] = 192;
    }

    for (int i = MAP_W; i < MAP_W * 2; i++)
    {
        cur_map_data[i] = 224;
    }

    ubox_wait_vsync();

    ubox_write_vm((uint8_t *)0x1800, MAP_W * MAP_H, cur_map_data);
}

void run_game()
{
    ubox_disable_screen();
    ubox_fill_screen(WHITESPACE_TILE);

    draw_map();

    ubox_enable_screen();

    while (1)
    {
        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
            break;

        ubox_wait();
    }
}