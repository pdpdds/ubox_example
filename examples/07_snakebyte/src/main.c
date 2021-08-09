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

volatile long g_tick_count = 0; //틱 카운트

int now()
{
	return g_tick_count;
}

void my_isr()
{
  
   g_tick_count++;
}


void draw_menu()
{
    uint8_t i;

    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    // game logo; starts in tile 32 and it is 10 x 3 tiles
    /*for (i = 0; i < 10; ++i)
    {
        ubox_put_tile(11 + i, 6, 32 + i);
        ubox_put_tile(11 + i, 7, 64 + i);
        ubox_put_tile(11 + i, 8, 96 + i);
    }*/

    put_text(13, 9, "TETRIS");
    put_text(8, 11, "PRESS SPACE BAR");

    //put_text(7, 2, "UBOX MSX LIB DEMO!");
    //put_text(4, 16, "CODE, GRAPHICS AND SOUND");
    //put_text(8, 17, "JUAN J. MARTINEZ");
    // 037 is ASCII 31 in octal, our Copyright sign
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

    put_text(3, 9, "GAME ALL CLEAR!");

    put_text(3, 12, "(PRESS ESC)");

    ubox_enable_screen();

    // wait until ESC is pressed
    while (1)
    {
        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
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

    // play game over music
    //mplayer_init(SONG, SONG_GAME_OVER);

    ubox_wait_for(128);

    ubox_disable_screen();
    ubox_fill_screen(WHITESPACE_TILE);
    ubox_enable_screen();

    g_gamestate = STATE_TITLE;

}       

void draw_stage_clear()
{
    ubox_disable_screen();

    put_text(11, 10, "STAGE CLEAR");

    ubox_enable_screen();

    // play game over music
    //mplayer_init(SONG, SONG_GAME_OVER);

    ubox_wait_for(128);

    ubox_disable_screen();
    ubox_fill_screen(WHITESPACE_TILE);
    ubox_enable_screen();

    g_gamestate = STATE_IN_GAME;
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

    ubox_enable_screen();

	ubox_set_user_isr(my_isr);
    // reg 1: activate sprites, v-blank int on, 16x16 sprites
    ubox_wvdp(1, 0xe2);


    draw_menu();


    // wait until fire is pressed
    // can be space (control will be cursors), or any fire button on a joystick
    while (1)
    {

        switch (g_gamestate)
        {
        case STATE_GAME_OVER:
            draw_game_over();
            break;
        case STATE_NO_MAP:
            draw_end_game();
            break; 
        case STATE_GAME_CLEAR:
            draw_stage_clear();
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


extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b, int fill)
{
	//if(fill)
		//rectfill(backBuffer, x1, y1, x2, y2, makecol(r, g, b));
	//else
		//rect(backBuffer, x1, y1, x2, y2, makecol(r, g, b));
}

extern void RenderCircle(int x, int y, int radius, int r, int g, int b)
{
	//circlefill(backBuffer, x, y, radius, makecol(r, g, b));
}

extern void RenderLine(int x, int y, int x2, int y2, int r, int g, int b)
{
	//line(backBuffer, x, y, x2, y2, makecol(r, g, b));
}

extern void RenderText(int x, int y, char* szMessage)
{
	//textout_ex(backBuffer, font, szMessage, x, y, makecol(0, 0, 0), -1);
}

extern void RenderTile(int x, int y, int x_count, int y_count, int tileNum)
{
    //for(int i = 0; i < y_count; i++)
       // for(int j = 0; j < x_count; j++)
            
            ubox_put_tile(x, y, tileNum);
            
            
            //ubox_put_tile(x, y, 42 + 2);
            //ubox_put_tile(x, y + 1, 74 + 0);
            //ubox_put_tile(x + 1, y + 1, 74 + 1);
            //ubox_put_tile(x + 2, y + 1, 74 + 2);

}

extern void EraseTile(int x, int y, int x_count, int y_count, int tileNum)
{
   ubox_put_tile(x, y, tileNum);
    //for(int i = 0; i < y_count; i++)
     //   for(int j = 0; j < x_count; j++)
          //  ubox_put_tile(x + j, y + i, tileNum);

}
 