#include <stdint.h>

#include "ubox.h"
#include "mplayer.h"

#include "helpers.h"
#include "game.h"

#define LOCAL
#include "main.h"

#include "tiles.h"

uint8_t g_gamestate = STATE_TITLE;
uint8_t g_stage = 0;

void draw_menu()
{
    uint8_t i;

    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    // game logo; starts in tile 32 and it is 10 x 3 tiles
    for (i = 0; i < 10; ++i)
    {
        ubox_put_tile(11 + i, 6, 32 + i);
        ubox_put_tile(11 + i, 7, 64 + i);
        ubox_put_tile(11 + i, 8, 96 + i);
    }

    put_text(11, 11, "PRESS FIRE");

    put_text(7, 2, "UBOX MSX LIB DEMO 2!");
    put_text(4, 16, "CODE, GRAPHICS AND SOUND");
    put_text(8, 17, "JUAN J. MARTINEZ");
    // 037 is ASCII 31 in octal, our Copyright sign
    put_text(8, 21, "\0372020 USEBOX.NET");

    ubox_enable_screen();
}

void draw_end_game()
{
    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    put_text(3, 9, "WELL DONE!");
    put_text(3, 10, "YOU HAVE FINISHED THE DEMO.");
    put_text(3, 12, "NOW GO AND MAKE GAMES :)");

    put_text(3, 14, "(PRESS ESC)");

    ubox_enable_screen();

    while (1)
    {
        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
            break;

        ubox_wait();
    }
}

void draw_game_over()
{
    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);
    put_text(11, 10, "GAME  OVER");

    ubox_enable_screen();

    mplayer_init(SONG, SONG_GAME_OVER);

    ubox_wait_for(128);
}

void draw_stage_clear()
{
    ubox_disable_screen();

    put_text(11, 10, "STAGE CLEAR");
    put_text(10, 11, "PRESS ANY KEY");

    ubox_enable_screen();

    ubox_wait_for(32);
    
    g_gamestate = STATE_IN_GAME;
    g_stage++;
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
    ubox_wvdp(1, 0xe2);

    mplayer_init(SONG, SONG_SILENCE);
    mplayer_init_effects(EFFECTS);
    ubox_set_user_isr(mplayer_play);

redraw_menu:
    draw_menu();
    
    while (1)
    {
        ctl = ubox_select_ctl();
        if (ctl != UBOX_MSX_CTL_NONE)
        {
            mplayer_play_effect_p(EFX_START, EFX_CHAN_NO, 0);
            ubox_wait_for(16);

            run_game(g_stage);

            if (!lives)
                draw_game_over();
            else 
                draw_stage_clear();    
         
        }
        ubox_wait();
    }
}
