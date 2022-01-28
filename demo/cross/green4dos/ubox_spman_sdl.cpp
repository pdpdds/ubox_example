


#if defined(WIN32) && !defined(HXWIN32)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif


//#include <vector>
//#include "spman.h"
struct sprite_attr {
    uint8_t y;
    uint8_t x;
    uint8_t pattern;
    uint8_t attr;
};

extern const unsigned char player_sprite[6][32];
extern const unsigned char enemy_sprite[3][32];
extern  "C" SDL_Surface *screen;
extern  "C" SDL_Surface* player_surface;
extern  "C" SDL_Surface* enemy_surface;

extern "C" SDL_Color sprite_pallete[16] =
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


const char max_pattern = 64;

struct pattern_info
{
    uint8_t* data;
    uint8_t pattern_length;
    uint8_t type;
    uint8_t image_per_sprite;
};

#ifdef SKYOS32
eastl::vector<pattern_info> g_pattern_info;
#else
//std::vector<pattern_info> g_pattern_info;
#endif

extern "C" void spman_init()
{
  //  g_pattern_info.clear();
}




extern "C" uint8_t spman_alloc_pat(uint8_t type, uint8_t* data, uint8_t len, uint8_t flip)
{

    return 0;
	/*std::vector<pattern_info>::iterator iter = g_pattern_info.begin();
	pattern_info info;

    int index = 0;
    for (; iter != g_pattern_info.end(); iter++)
    {
        if (iter->data == data)
            return index * max_pattern;

        index++;
    }

	info.data = data;	
	info.pattern_length = 64;
	info.type = type;
	info.pattern_length = (uint8_t)(len / 3);
	
    g_pattern_info.push_back(info);

    return index * max_pattern;*/
}

extern "C" void spman_alloc_sprite(struct sprite_attr* sp);

extern "C" void spman_alloc_fixed_sprite(struct sprite_attr* sp)
{
    spman_alloc_sprite(sp);
}



void put_pixel(SDL_Surface* screen,int x,int y,SDL_Color* p)
{
  /* Get a pointer to the video surface's pixels in memory. */
    Uint32 *pixels = (Uint32*) screen->pixels;

    /* Calculate offset to the location we wish to write to */
    int offset = (screen->pitch / sizeof(Uint32)) * y + x;

    /* Compose RGBA values into correct format for video surface and copy to screen */
    *(pixels + offset) = SDL_MapRGBA(screen->format, p->r, p->g, p->b, 255); 
}

extern "C" void spman_alloc_sprite(struct sprite_attr* sp)
{
    /*uint8_t sprite_index = sp->pattern / max_pattern;
    uint8_t pattern_index = sp->pattern % max_pattern;
    pattern_info iter = g_pattern_info[sprite_index];
    uint8_t* data = iter.data;


    uint8_t temp = (iter.image_per_sprite * 3) * 4;

    char flip = 0;
    if (pattern_index >= temp)
    {
        pattern_index -= temp;
        flip = 1;
    }

    pattern_index = pattern_index / 4;
      

    uint8_t color = 0;
    for (int i = 0; i < 32; i++)
    {
       
        color = data[pattern_index * 32 + i];

		SDL_LockSurface(screen);
        for (int j = 0; j < 8; j++)
        {
            uint8_t mask = 0;
            
            mask = 1 << (7 - j);

            if (color & mask)
            {
				
				//put_pixel(screen, sprite_pallete[sp->attr].r, sprite_pallete[sp->attr].g, sprite_pallete[sp->attr].b, 255
               // SDL_SetRenderDrawColor(g_renderer, sprite_pallete[sp->attr].r, sprite_pallete[sp->attr].g, sprite_pallete[sp->attr].b, 255);

                if(!flip)
					put_pixel(screen, sp->x + ((i / 16) * 8 + j), sp->y + (i % 16), &sprite_pallete[sp->attr]);
                    //SDL_RenderDrawPoint(g_renderer, sp->x + ((i / 16) * 8 + j), sp->y + (i % 16));
                //else
					put_pixel(screen, sp->x + 16 - ((i / 16) * 8 + j), sp->y + (i % 16), &sprite_pallete[sp->attr]);
                    //SDL_RenderDrawPoint(g_renderer, sp->x + 16 - ((i / 16) * 8 + j), sp->y + (i % 16));
				

            }            
        }

		SDL_UnlockSurface(screen);
    }*/
}

extern "C" void spman_sprite_flush()
{

}


extern "C" void spman_update()
{    
	SDL_Flip(screen);
    //SDL_RenderPresent(g_renderer);
}

extern "C" void spman_hide_all_sprites()
{

}
