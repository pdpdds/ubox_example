#include <stdint.h>
#include "ubox.h"
#include "mplayer.h"
#include "game.h"

#define LOCAL
#include "tiles.h"

#define WHITESPACE_TILE 129
uint8_t ctl;

uint8_t g_gamestate = STATE_TITLE;

void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}

void draw_menu()
{
    uint8_t i;

    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    put_text(11, 9, "BREAK OUT");
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

void draw_end_game()
{
    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    put_text(3, 9, "GAME CLEAR!");

    put_text(3, 12, "(PRESS ESC)");

    ubox_enable_screen();
    while (1)
    {
         ctl = ubox_select_ctl();
        
        if (ctl == UBOX_MSX_CTL_NONE)
            break;
        
        ubox_wait();
    }

    g_gamestate = STATE_TITLE;
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

void InitEnvironnmet()
{
    ubox_init_isr(2);

    ubox_set_mode(2);
    ubox_set_colors(1, 1, 1);

    ubox_disable_screen();

    ubox_set_tiles(tiles);
    ubox_set_tiles_colors(tiles_colors);

    ubox_fill_screen(WHITESPACE_TILE);

    ubox_enable_screen();

    ubox_wvdp(1, 0xe2);
}

void main()
{
    
    InitEnvironnmet();

    draw_menu();

    while (1)
    {

        switch (g_gamestate)
        {
        case STATE_GAME_OVER:
            draw_game_over();
            break;
        case STATE_GAME_CLEAR:
            draw_end_game();
            break;
        case STATE_IN_GAME:
            run_game();
            break;
        case STATE_TITLE:
            draw_menu();
            break;
        }

        ubox_wait();
    }
}


extern void RenderTile(int x, int y, int x_count, int y_count, int tileNum)
{
    
            ubox_put_tile(x, y, 42 + 0);
            ubox_put_tile(x, y + 1, 74 + 0);

            ubox_put_tile(x + 1, y, 42 + 1);
            ubox_put_tile(x + 1, y + 1, 74 + 1);
    
            ubox_put_tile(x + 2, y, 42 + 2);
            ubox_put_tile(x + 2, y + 1, 74 + 2);
            
       

}

extern void EraseTile(int x, int y, int x_count, int y_count, int tileNum)
{
   
    for(int i = 0; i < y_count; i++)
        for(int j = 0; j < x_count; j++)
            ubox_put_tile(x + j, y + i, tileNum);

}
 