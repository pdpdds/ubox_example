#include "spman.h"
#include <SDL2/SDL.h>

extern SDL_Renderer* g_renderer;

extern const unsigned char player_sprite[6][32];
extern const unsigned char enemy_sprite[3][32];

SDL_Color sprite_pallete[16] =
{
    {255, 0, 255, 0},
    {0, 0, 0, 0},
    {102, 204, 102, 0 },
    {136, 238, 136, 0},
    {68, 68, 221, 0},
    {119, 119, 255, 0},
    {187, 85, 85, 0},
    {119, 221, 221, 0},
    {221, 102, 102, 0},
    {255, 119, 119, 0},
    {204, 204, 85, 0},
    {238, 238, 136, 0},
    {85, 170, 85, 0},
    {187, 85, 187, 0},
    {204, 204, 204, 0},
    {238, 238, 238, 0},
};

void spman_init()
{

}

uint8_t spman_alloc_pat(uint8_t type, uint8_t* data, uint8_t len, uint8_t flip)
{
	return 0;
}

void spman_alloc_fixed_sprite(struct sprite_attr* sp)
{
    uint8_t pattern = sp->pattern / 4;
    uint8_t color = 0;
    char flip = 0;

    for (int i = 0; i < 32; i++)
    {
        if (pattern >= 3)
        {
            pattern -= 3;
            flip = 1;
        }

        //if(pattern == 1)
             color = player_sprite[pattern][i];
        //else
          //  color = player_sprite[1][i];

        for (int j = 0; j < 8; j++)
        {
            uint8_t mask = 1 << (7 - j);
            if (color & mask)
            {
                SDL_SetRenderDrawColor(g_renderer, sprite_pallete[sp->attr].r, sprite_pallete[sp->attr].g, sprite_pallete[sp->attr].b, 255);

                if (!flip)
                    SDL_RenderDrawPoint(g_renderer, sp->x + ((i / 16) * 8 + j), sp->y + (i % 16));
                else
                    SDL_RenderDrawPoint(g_renderer, sp->x + 16 - ((i / 16) * 8 + j), sp->y + (i % 16));                    
            }            
        }
    }
}

void spman_alloc_sprite(struct sprite_attr* sp)
{
    uint8_t pattern = sp->pattern / 4;
    uint8_t color = 0;
    char flip = 0;
    for (int i = 0; i < 32; i++)
    {
        if (pattern >= 3)
        {
            pattern -= 3;
            flip = 1;
        }
        
        color = enemy_sprite[pattern][i];

        for (int j = 0; j < 8; j++)
        {
            uint8_t mask = 0;
            
            mask = 1 << (7 - j);

            if (color & mask)
            {
                SDL_SetRenderDrawColor(g_renderer, sprite_pallete[sp->attr].r, sprite_pallete[sp->attr].g, sprite_pallete[sp->attr].b, 255);

                if(!flip)
                    SDL_RenderDrawPoint(g_renderer, sp->x + ((i / 16) * 8 + j), sp->y + (i % 16));
                else
                    SDL_RenderDrawPoint(g_renderer, sp->x + 16 - ((i / 16) * 8 + j), sp->y + (i % 16));
            }            
        }
    }
}

void spman_sprite_flush()
{

}

void spman_update()
{    
    SDL_RenderPresent(g_renderer);
}

void spman_hide_all_sprites()
{

}
