#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <IMG_savepng.h>

typedef struct tag_Color
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}Color;

SDL_Surface* LoadPNG(const char* filename);
Uint32 GetPixel(SDL_Surface* surface, int x, int y);
bool SetPixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b);
int GetDistance(Color current, Color match);
int FindNearestColor(Color* map, Color current);



char g_tiles_rgb[2048 * 8 * 4];

extern "C" Color sprite_pallete[16] =
{
	{255, 0, 255},
	{0, 0, 0},
	{102, 204, 102},
	{136, 238, 136},
	{68, 68, 221},
	{119, 119, 255},
	{187, 85, 85},
	{119, 221, 221},
	{221, 102, 102},
	{255, 119, 119},
	{204, 204, 85},
	{238, 238, 136},
	{85, 170, 85},
	{187, 85, 187},
	{204, 204, 204},
	{238, 238, 238},
};

#undef main
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		SDL_Log("Usage : tileconverter <filename>\n");
		return 0;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return 0;
	}

	SDL_Surface* surface = LoadPNG(argv[1]);

	if (surface == 0)
	{
		SDL_Log("can not open file or is not PNG File\n");
		return 0;
	}

	if (surface->w != 256 || surface->h != 64)
	{
		SDL_Log("The surface's width : %d height : %d", surface->w, surface->h);
		return 0;
	}

	SDL_PixelFormat* pixelFormat = surface->format;
	Uint32 pixelFormatEnum = pixelFormat->format;

	
	const char* surfacePixelFormatName = SDL_GetPixelFormatName(pixelFormatEnum);
	if (SDL_PIXELFORMAT_RGB24 != pixelFormatEnum)
		if (SDL_PIXELFORMAT_ABGR8888 != pixelFormatEnum)
	{
		SDL_Log("The surface's pixelformat is %s", surfacePixelFormatName);
		return 0;
	}


	uint32_t rmask = 0x000000ff;
	uint32_t gmask = 0x0000ff00;
	uint32_t bmask = 0x00ff0000;
	uint32_t amask = 0;

	SDL_Surface* dest_surface = SDL_CreateRGBSurfaceWithFormatFrom(g_tiles_rgb, 256, 64, 24, 3 * 256,
		SDL_PIXELFORMAT_RGB24);

	if (dest_surface == NULL) {
		SDL_Log("Creating surface failed: %s", SDL_GetError());
		exit(1);
	}

	for(int y= 0; y < 64; y++)
		for (int x = 0; x < 256; x++)
		{
			Uint32 pixel = GetPixel(surface, x, y);

			Color current;
			current.R = pixel;
			current.G = pixel >> 8;
			current.B = pixel >> 16;

			int index = FindNearestColor(sprite_pallete, current);
			SetPixel(dest_surface, x, y, sprite_pallete[index].R, sprite_pallete[index].G, sprite_pallete[index].B);
		}

	char output[256] = "24bit_";
	strcat(output, argv[1]);

	IMG_Custom_SavePNG(output, dest_surface, -1);

	SDL_Log("%s Generated!!", output);

	SDL_FreeSurface(dest_surface);
	
	return 0;
}

SDL_Surface* LoadPNG(const char* filename)
{
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return 0;
	}

	SDL_Surface* surface = IMG_Load(filename);

	return surface;
}

Uint32 GetPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;

	SDL_assert(bpp == 3);

	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;


	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		return p[0] << 16 | p[1] << 8 | p[2];
	
	return p[0] | p[1] << 8 | p[2] << 16;		
}

bool SetPixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	int bpp = surface->format->BytesPerPixel;

	SDL_assert(bpp == 3);

	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;


	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		p[0] = b;
		p[1] = g;
		p[2] = r;
	}
	else
	{
		p[0] = r;
		p[1] = g;
		p[2] = b;
	}

	return true;
}


	
int GetDistance(Color current, Color match)
{
	int redDifference;
	int greenDifference;
	int blueDifference;

	redDifference = current.R - match.R;
	greenDifference = current.G - match.G;
	blueDifference = current.B - match.B;

	return redDifference * redDifference + greenDifference * greenDifference +
		blueDifference * blueDifference;
}

int FindNearestColor(Color* map, Color current)
{
	int shortestDistance;
	int index;

	index = -1;
	shortestDistance = 0x00FFFFFF;

	for (int i = 0; i < 16; i++)
	{
		Color match;
		int distance;

		match = map[i];
		distance = GetDistance(current, match);

		if (distance < shortestDistance)
		{
			index = i;
			shortestDistance = distance;
		}
	}

	return index;
}