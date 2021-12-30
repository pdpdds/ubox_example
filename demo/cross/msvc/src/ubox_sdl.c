#include "ubox.h"

#if defined(WIN32)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

SDL_Window* g_window;
SDL_Renderer* g_renderer;
SDL_Texture* g_tile_texture;

uint32_t screen_width = 640;
uint32_t screen_height = 480;

uint32_t map_width = 32;
uint32_t map_height = 21;

int32_t g__tick_interval = 73;

extern SDL_Color sprite_pallete[16];

void set_icon()
{
	SDL_Surface* surface;     

	//SDL Icon
	Uint16 pixels[16 * 16] = 
	{  
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0aab, 0x0789, 0x0bcc, 0x0eee, 0x09aa, 0x099a, 0x0ddd,
	  0x0fff, 0x0eee, 0x0899, 0x0fff, 0x0fff, 0x1fff, 0x0dde, 0x0dee,
	  0x0fff, 0xabbc, 0xf779, 0x8cdd, 0x3fff, 0x9bbc, 0xaaab, 0x6fff,
	  0x0fff, 0x3fff, 0xbaab, 0x0fff, 0x0fff, 0x6689, 0x6fff, 0x0dee,
	  0xe678, 0xf134, 0x8abb, 0xf235, 0xf678, 0xf013, 0xf568, 0xf001,
	  0xd889, 0x7abc, 0xf001, 0x0fff, 0x0fff, 0x0bcc, 0x9124, 0x5fff,
	  0xf124, 0xf356, 0x3eee, 0x0fff, 0x7bbc, 0xf124, 0x0789, 0x2fff,
	  0xf002, 0xd789, 0xf024, 0x0fff, 0x0fff, 0x0002, 0x0134, 0xd79a,
	  0x1fff, 0xf023, 0xf000, 0xf124, 0xc99a, 0xf024, 0x0567, 0x0fff,
	  0xf002, 0xe678, 0xf013, 0x0fff, 0x0ddd, 0x0fff, 0x0fff, 0xb689,
	  0x8abb, 0x0fff, 0x0fff, 0xf001, 0xf235, 0xf013, 0x0fff, 0xd789,
	  0xf002, 0x9899, 0xf001, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0xe789,
	  0xf023, 0xf000, 0xf001, 0xe456, 0x8bcc, 0xf013, 0xf002, 0xf012,
	  0x1767, 0x5aaa, 0xf013, 0xf001, 0xf000, 0x0fff, 0x7fff, 0xf124,
	  0x0fff, 0x089a, 0x0578, 0x0fff, 0x089a, 0x0013, 0x0245, 0x0eff,
	  0x0223, 0x0dde, 0x0135, 0x0789, 0x0ddd, 0xbbbc, 0xf346, 0x0467,
	  0x0fff, 0x4eee, 0x3ddd, 0x0edd, 0x0dee, 0x0fff, 0x0fff, 0x0dee,
	  0x0def, 0x08ab, 0x0fff, 0x7fff, 0xfabc, 0xf356, 0x0457, 0x0467,
	  0x0fff, 0x0bcd, 0x4bde, 0x9bcc, 0x8dee, 0x8eff, 0x8fff, 0x9fff,
	  0xadee, 0xeccd, 0xf689, 0xc357, 0x2356, 0x0356, 0x0467, 0x0467,
	  0x0fff, 0x0ccd, 0x0bdd, 0x0cdd, 0x0aaa, 0x2234, 0x4135, 0x4346,
	  0x5356, 0x2246, 0x0346, 0x0356, 0x0467, 0x0356, 0x0467, 0x0467,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
	  0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff
	};
	surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2, 0x0f00, 0x00f0, 0x000f, 0xf000);

	SDL_SetWindowIcon(g_window, surface);
	SDL_FreeSurface(surface);
}

void ubox_set_mode(uint8_t mode) 
{
#if defined(__ANDROID__)
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
#elif defined(WIN32)
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
#else
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
#endif
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return;
	}

#if defined(__ANDROID__)
	g_window = SDL_CreateWindow("GREEN for Android", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 448, SDL_WINDOW_FULLSCREEN);
#else
	g_window = SDL_CreateWindow("GREEN for WIN32", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);
#endif

	if (g_window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return;
	}

#if defined(__ANDROID__)
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
#elif defined(WIN32)
	set_icon();
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
#else
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE);
#endif

	if (g_renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return;
	}

	SDL_RenderSetLogicalSize(g_renderer, map_width * 8, (map_height + 4) * 8);

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


void load_png(const char* filename)
{	
	if (g_renderer == 0)
		return;

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return;
	}

	SDL_Surface* surface = IMG_Load(filename);

	if (surface == 0)
		return;

	g_tile_texture = SDL_CreateTextureFromSurface(g_renderer, surface);

	if (g_tile_texture == 0)
		return;

	SDL_FreeSurface(surface);
}

extern const unsigned char tiles_colors[2048];

char g_tiles_rgb[2048 * 8 * 4];

void ubox_set_tiles(uint8_t* tiles)
{
#if 0
	load_png("tiles.png");
#else
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				uint8_t tile_pixels = tiles[(i * 8 * 32) + (j * 8) + k];
				uint8_t tile_color = tiles_colors[(i * 8 * 32) + (j * 8) + k];

				uint8_t color = (tile_color >> 4);
			
				uint8_t color2 = (tile_color << 4);
				color2 = color2 >> 4;

				for (uint8_t t = 0; t < 8; t++)
				{
					uint8_t mask = 1 << (7 - t);
					int index = ((i * 8 * 8 * 32) + (k) * 8 * 32 + (j * 8 + t)) * 3;


					if (tile_pixels & mask)
					{
						g_tiles_rgb[index] = sprite_pallete[color].r;
						g_tiles_rgb[index + 1] = sprite_pallete[color].g;
						g_tiles_rgb[index + 2] = sprite_pallete[color].b;
					}
					else
					{
						g_tiles_rgb[index] = sprite_pallete[color2].r;
						g_tiles_rgb[index + 1] = sprite_pallete[color2].g;
						g_tiles_rgb[index + 2] = sprite_pallete[color2].b;
					}
				}
			}
		}
	}
	
	uint32_t rmask = 0x000000ff;
	uint32_t gmask = 0x0000ff00;
	uint32_t bmask = 0x00ff0000;
	uint32_t amask = 0;

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)g_tiles_rgb, 256, 64, 24, 3 * 256,
		rmask, gmask, bmask, amask);

	if (surface == NULL) {
		SDL_Log("Creating surface failed: %s", SDL_GetError());		
		exit(1);
	}

	g_tile_texture = SDL_CreateTextureFromSurface(g_renderer, surface);

	if (g_tile_texture == 0)
		return;

	SDL_FreeSurface(surface);

#endif

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
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	
	for (uint8_t y = 0; y < map_height + 4; y++)
		for (uint8_t x = 0; x < map_width; x++)
		{
			ubox_put_tile(x, y, tile);
		}

}

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
	uint32_t delay = (frames / 60.0f) * 1000;
	SDL_Delay(delay);
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

uint8_t control_key = 0;
uint8_t read_key_7 = 0;

uint8_t ubox_select_ctl()
{
	control_key = 0;
	read_key_7 = 0;

	SDL_Event event;
	//char exit_loop = 0;
	//if (!exit_loop)
	//{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE: return UBOX_MSX_CTL_CURSOR;
				case SDLK_m: return UBOX_MSX_CTL_CURSOR;
#if defined(__ANDROID__) || defined(WIN32)
				case SDLK_ESCAPE: return UBOX_MSX_CTL_EXIT;
#endif
				}
				//exit_loop = 1;
				break;
			}

			//if (exit_loop)
			//	break;
		}
	//}

	return UBOX_MSX_CTL_NONE;
}


void read_key()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
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
			case SDLK_ESCAPE:
				read_key_7 |= UBOX_MSX_KEY_ESC;
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
			case SDLK_ESCAPE:
				read_key_7 &= ~(UBOX_MSX_KEY_ESC);
				break;
			}
			break;
		}
	}
}

uint8_t ubox_read_ctl(uint8_t control)
{
	read_key();
	return control_key;
}

uint8_t ubox_read_keys(uint8_t row)
{
	read_key();
	return read_key_7;
}


size_t GetMapDataSize(const uint8_t* map)
{
	size_t count = 1;

	while (*map != 0xff)
	{
		count++;
		map++;
	}

	return count;
}

#include "expand.h"

void ap_uncompress(const uint8_t* dst, const uint8_t* src)
{
	size_t n = GetMapDataSize(src - 3);
	apultra_decompress(src, dst, n - 3, 672, 0, 0);
}