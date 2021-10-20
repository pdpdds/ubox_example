#include "ubox.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* g_window;
SDL_Renderer* g_renderer;
SDL_Texture* g_tile_texture;

uint32_t screen_width = 640;
uint32_t screen_height = 480;

uint32_t map_width = 32;
uint32_t map_height = (21);

void ubox_set_mode(uint8_t mode) 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return;
	}

	g_window = SDL_CreateWindow("GREEN for WIN32", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	if (g_window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return;
	}

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_RenderSetLogicalSize(g_renderer, map_width * 8, (map_height + 4) * 8);

	if (g_renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return;
	}

	return;
}

void ubox_enable_screen()
{
	SDL_RenderPresent(g_renderer);
}


void ubox_disable_screen()
{

}

void ubox_set_colors(uint8_t fg, uint8_t bg, uint8_t border)
{

}

void ubox_write_vm(uint8_t* dst, uint16_t len, uint8_t* src)
{
	for (uint8_t y = 0; y < map_height; y++)
		for (uint8_t x = 0; x < map_width; x++)
		{
			uint32_t tile = src[x + y * map_width];
			ubox_put_tile(x, y, tile);
		}

}

void ubox_read_vm(uint8_t* dst, uint16_t len, uint8_t* src)
{

}


void ubox_wvdp(uint8_t reg, uint8_t data)
{

}


uint8_t ubox_get_vsync_freq()
{
	return 0;
}


void ubox_set_tiles(uint8_t* tiles)
{
	const char* tilename = "tiles.png";
	if (g_renderer == 0)
		return;

	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return;
	}

	SDL_Surface* surface = IMG_Load(tilename);

	if (surface == 0)
		return;

	g_tile_texture = SDL_CreateTextureFromSurface(g_renderer, surface);

	if (g_tile_texture == 0)
		return;

	SDL_FreeSurface(surface);	
}

void ubox_set_tiles_colors(uint8_t* colors)
{
}

void ubox_put_tile(uint8_t x, uint8_t y, uint8_t tile)
{
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	uint8_t src_x = tile % map_width;
	uint8_t src_y = tile / map_width;


	srcRect.x = src_x * 8;
	srcRect.y = src_y * 8;
	srcRect.w = 8;
	srcRect.h = 8;


	dstRect.x = x * 8;
	dstRect.y = y * 8;
	dstRect.w = 8;
	dstRect.h = 8;

	SDL_RenderCopy(g_renderer, g_tile_texture, &srcRect, &dstRect);
}

uint8_t ubox_get_tile(uint8_t x, uint8_t y)
{
	return 0;
}

void ubox_fill_screen(uint8_t tile)
{

}

int32_t g__tick_interval = 73;

void ubox_init_isr(uint8_t wait_ticks)
{

}

void ubox_set_user_isr(void (*fn)())
{

}

Uint32 TimeLeft(void)
{
	static Uint32 next_time = 0;
	Uint32 now;

	now = SDL_GetTicks();
	if (next_time <= now) {
		next_time = now + g__tick_interval;
		return(0);
	}
	return (next_time - now);
}

void ubox_wait()
{
	SDL_Delay(TimeLeft());
}


void ubox_wait_for(uint8_t frames)
{
	//SDL_Delay(delay);
}

void ubox_reset_tick()
{

}

void ubox_set_sprite_pat8(uint8_t* data, uint8_t pattern)
{

}

void ubox_set_sprite_pat8_flip(uint8_t* data, uint8_t pattern)
{

}

void ubox_set_sprite_attr(struct sprite_attr* attr, uint8_t sprite)
{

}


void ubox_set_sprite_pat16(uint8_t* data, uint8_t pattern)
{

}

void ubox_set_sprite_pat16_flip(uint8_t* data, uint8_t pattern)
{

}

uint8_t ubox_select_ctl()
{
	SDL_Event event;
	char exit = 0;
	while (!exit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE: return UBOX_MSX_CTL_CURSOR;
				case SDLK_m: return UBOX_MSX_CTL_CURSOR;

				}
				exit = 1;
				break;
			}

			if (exit)
				break;
		}
	}

	return UBOX_MSX_CTL_NONE;
}

uint8_t control_key = 0;

uint8_t ubox_read_ctl(uint8_t control)
{
	SDL_Event event;

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				control_key |= UBOX_MSX_CTL_FIRE1;
				break;
			case SDLK_m:
				control_key |= UBOX_MSX_CTL_FIRE2;
				break;
			case SDLK_UP:
				control_key |= UBOX_MSX_CTL_UP;
				break;
			case SDLK_LEFT:
				control_key |= UBOX_MSX_CTL_LEFT;
				break;
			case SDLK_RIGHT:
				control_key |= UBOX_MSX_CTL_RIGHT;
				break;
			case SDLK_DOWN:
				control_key |= UBOX_MSX_CTL_DOWN;
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				control_key &= ~(UBOX_MSX_CTL_FIRE1);
				break;
			case SDLK_m:
				control_key &= ~(UBOX_MSX_CTL_FIRE2);
				break;
			case SDLK_UP:
				control_key &= ~(UBOX_MSX_CTL_UP);
				break;
			case SDLK_LEFT:
				control_key &= ~(UBOX_MSX_CTL_LEFT);
				break;
			case SDLK_RIGHT:
				control_key &= ~(UBOX_MSX_CTL_RIGHT);
				break;
			case SDLK_DOWN:
				control_key &= ~(UBOX_MSX_CTL_DOWN);
				break;
			}
			break;
		}
	}

	return control_key;
}

uint8_t ubox_read_keys(uint8_t row)
{
	return 0;
}