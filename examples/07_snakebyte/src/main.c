#include <stdint.h>
#include "ubox.h"
#include "game.h"
#include "util.h"

#define LOCAL
#include "tiles.h"

uint8_t ctl;

uint8_t g_gamestate = STATE_TITLE;


void draw_title()
{    
    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    put_text(11, 9, "SNAKEBYTE");
    put_text(8, 11, "PRESS SPACE BAR");

    put_text(8, 18, "\0372021 YUZA SOFT");

    ubox_enable_screen();

    while (1)
    {
        ctl = ubox_select_ctl();

        if (ctl != UBOX_MSX_CTL_NONE)
            break;

        ubox_wait();
    }

    g_gamestate = STATE_IN_GAME;
}

void draw_game_over()
{
    ubox_disable_screen();

    put_text(11, 10, "GAME  OVER");

    ubox_enable_screen();

    ubox_wait_for(128);

    ubox_disable_screen();
    ubox_fill_screen(WHITESPACE_TILE);
    ubox_enable_screen();

    g_gamestate = STATE_TITLE;
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

    ubox_set_user_isr(my_isr);
    ubox_wvdp(1, 0xe2);

    draw_title();

    while (1)
    {
        switch (g_gamestate)
        {
        case STATE_GAME_OVER:
            draw_game_over();
            break;
        case STATE_IN_GAME:
            run_game();
            break;
        case STATE_TITLE:
            draw_title();
            break;
        }

        ubox_wait();
    }
}
