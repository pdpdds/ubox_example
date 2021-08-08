#include <stdint.h>
#include "ubox.h"
#include "game.h"
#include "spman.h"

#define WHITESPACE_TILE 129

#define SOCOBAN_MAX_WIDTH 20
#define SOCOBAN_MAX_HEIGHT 20

#define tileSize 8

#define GAME_WIDTH 256
#define GAME_HEIGHT 128

#define SDL_RATIO_X(width) GAME_WIDTH
#define SDL_RATIO_Y(height) GAME_HEIGHT

#define SDL_REVERSE_RATIO_X(width) GAME_WIDTH
#define SDL_REVERSE_RATIO_Y(height) GAME_HEIGHT

enum enumSpaceProperty
{
	ENUM_SPACE_EMPTY,
	ENUM_SPACE_BLOCK,
	ENUM_SPACE_BOX_POINT,
	ENUM_SPACE_BOX,
	ENUM_SPACE_ENEMY,
};

int g_screen_width = 0;
int g_screen_height = 0;

struct Rect
{
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
};

struct PlayerInfo
{
	uint8_t x;
	uint8_t y;
	uint8_t old_x;
	uint8_t old_y;
};

struct MapInfo
{
	uint8_t stageNum;
	uint8_t BoxCount;
	unsigned char pMapPointer[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT];
	unsigned char boxInfo[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT];
};

const unsigned char g_map1[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 3, 1, 3, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

const unsigned char g_map2[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 2, 0, 0, 2, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 1, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

const unsigned char g_map3[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 3, 3, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

const unsigned char g_map4[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 1, 2, 1, 0, 0, 0, 0, 1, 0, 3, 0, 2, 1, 0, 0, 1, 0, 0},
	{0, 0, 1, 2, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 3, 0, 1, 0, 0},
	{0, 0, 1, 3, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
	{0, 0, 1, 0, 3, 0, 1, 0, 0, 1, 0, 1, 2, 0, 0, 3, 0, 1, 0, 0},
	{0, 0, 1, 2, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

const unsigned char g_map5[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 3, 0, 0, 2, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 1, 2, 1, 3, 0, 1, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 2, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 1, 2, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 3, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 2, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

struct MapInfo g_mapInfo = {0, 0};
struct PlayerInfo g_playerInfo;
char g_bLevelReset = 0;

struct sprite_attr sp;
extern uint8_t g_gamestate;

void draw_map()
{
	DrawBoard();
	DrawWall();
}

void run_game()
{

	g_gamestate = STATE_IN_GAME;

	if (g_mapInfo.stageNum >= 5)
	{
		g_gamestate = STATE_NO_MAP;
		g_mapInfo.stageNum = 0;
		return;
	}

	InitGame(32 * 8, 21 * 8);

	spman_init();

	ubox_disable_screen();

	ubox_fill_screen(WHITESPACE_TILE);

	ubox_enable_screen();

	draw_map();

	while (1)
	{
		if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
			break;

		ProcessLogic();

		DrawWorld();

		ubox_wait();

		spman_update();

		if (g_gamestate == STATE_GAME_OVER || g_gamestate == STATE_GAME_CLEAR)
			break;

		if (g_bLevelReset == 0)
			run_game();
	}

	spman_hide_all_sprites();
}

int InRect(int posx, int posy, struct Rect *rect)
{
	if (posx >= rect->x1 && posx < rect->x2 && posy >= rect->y1 && posy < rect->y2)
		return 1;

	return 0;
}

void DrawBoard()
{
	ubox_disable_screen();

	for (int index = 0; index < SOCOBAN_MAX_WIDTH; index++)
		RenderTile(index, 0, 1, 0, 74);

	for (int index = 0; index < SOCOBAN_MAX_WIDTH; index++)
		RenderTile(index, SOCOBAN_MAX_HEIGHT - 1, 1, 0, 74);

	for (int index = 1; index < SOCOBAN_MAX_HEIGHT - 1; index++)
		RenderTile(0, index, 1, 0, 74);

	for (int index = 0; index < SOCOBAN_MAX_HEIGHT - 1; index++)
		RenderTile(SOCOBAN_MAX_WIDTH - 1, index, 1, 0, 74);

	ubox_enable_screen();
}

void DrawGround()
{
}

void InitGame(int screen_width, int screen_height)
{
	g_screen_width = screen_width;
	g_screen_height = screen_height;

	for (int x = 0; x < SOCOBAN_MAX_WIDTH; x++)
	{
		for (int y = 0; y < SOCOBAN_MAX_HEIGHT; y++)
		{

			if (g_mapInfo.stageNum == 0)
				g_mapInfo.pMapPointer[x][y] = g_map1[x][y];

			else if (g_mapInfo.stageNum == 1)
				g_mapInfo.pMapPointer[x][y] = g_map2[x][y];

			else if (g_mapInfo.stageNum == 2)
				g_mapInfo.pMapPointer[x][y] = g_map3[x][y];
			else if (g_mapInfo.stageNum == 3)
				g_mapInfo.pMapPointer[x][y] = g_map4[x][y];
			else if (g_mapInfo.stageNum == 4)
				g_mapInfo.pMapPointer[x][y] = g_map5[x][y];
		}
	}

	if (0 == g_bLevelReset)
	{
		if (g_mapInfo.stageNum == 3 || g_mapInfo.stageNum == 4)
		{
			g_playerInfo.x = 7;
			g_playerInfo.y = 9;
		}
		else
		{
			g_playerInfo.x = 7;
			g_playerInfo.y = 11;
	
		}
		
		g_playerInfo.old_x = 0;
		g_playerInfo.old_y = 0;

		g_bLevelReset = 1;

		for (int y = 0; y < SOCOBAN_MAX_HEIGHT; y++)
		{
			for (int x = 0; x < SOCOBAN_MAX_WIDTH; x++)
			{
				if (g_mapInfo.pMapPointer[y][x] == ENUM_SPACE_BOX)
				{
					g_mapInfo.boxInfo[y][x] = g_mapInfo.pMapPointer[y][x];
					g_mapInfo.BoxCount++;
				}
				else
				{
					g_mapInfo.boxInfo[y][x] = ENUM_SPACE_EMPTY;
				}
			}
		}
	}
}

char IsCanGo(const char iX, const char iY)
{
	if (g_mapInfo.pMapPointer[g_playerInfo.y][g_playerInfo.x] == ENUM_SPACE_BLOCK)
		return 0;

	if (g_mapInfo.boxInfo[g_playerInfo.y][g_playerInfo.x] == ENUM_SPACE_BOX)
	{
		if (g_mapInfo.pMapPointer[g_playerInfo.y + iY][g_playerInfo.x + iX] == ENUM_SPACE_BLOCK ||
			g_mapInfo.boxInfo[g_playerInfo.y + iY][g_playerInfo.x + iX] == ENUM_SPACE_BOX)
		{
			return 0;
		}

		g_mapInfo.boxInfo[g_playerInfo.y][g_playerInfo.x] = ENUM_SPACE_EMPTY;
		g_mapInfo.boxInfo[g_playerInfo.y + iY][g_playerInfo.x + iX] = ENUM_SPACE_BOX;
	}

	return 1;
}

char ProcessDirective(char iX, char iY)
{
	if (iX == 0 && iY == 0)
		return 0;

	g_playerInfo.old_x = g_playerInfo.x;
	g_playerInfo.old_y = g_playerInfo.y;

	g_playerInfo.x += iX;
	g_playerInfo.y += iY;

	if (IsCanGo(iX, iY) == 0)
	{
		g_playerInfo.x -= iX;
		g_playerInfo.y -= iY;
		return 0;
	}

	return 1;
}

void ProcessInput()
{
	if (ubox_read_keys(8) == UBOX_MSX_KEY_LEFT)
		ProcessDirective(-1, 0);

	if (ubox_read_keys(8) == UBOX_MSX_KEY_RIGHT)
		ProcessDirective(1, 0);

	if (ubox_read_keys(8) == UBOX_MSX_KEY_UP)
		ProcessDirective(0, -1);

	if (ubox_read_keys(8) == UBOX_MSX_KEY_DOWN)
		ProcessDirective(0, 1);

	if (ubox_read_keys(4) == UBOX_MSX_KEY_P)
	{
		g_mapInfo.BoxCount = 0;
		g_bLevelReset = 0;
	}
}

void UpdateGameStatus()
{
	int iCurrentExactBoxCount = 0;

	for (int y = 0; y < SOCOBAN_MAX_HEIGHT; y++)
		for (int x = 0; x < SOCOBAN_MAX_WIDTH; x++)
		{
			if (g_mapInfo.boxInfo[y][x] == ENUM_SPACE_BOX && g_mapInfo.pMapPointer[y][x] == ENUM_SPACE_BOX_POINT)
				iCurrentExactBoxCount++;
		}
	if (iCurrentExactBoxCount == g_mapInfo.BoxCount)
	{
		g_mapInfo.stageNum++;
		g_mapInfo.BoxCount = 0;
		g_bLevelReset = 0;
		g_gamestate = STATE_GAME_CLEAR;
	}
}

void DrawWall()
{
	ubox_disable_screen();

	for (int y = 1; y < SOCOBAN_MAX_HEIGHT - 1; y++)
	{
		for (int x = 1; x < SOCOBAN_MAX_WIDTH - 1; x++)
		{
			if (g_mapInfo.pMapPointer[y][x] == ENUM_SPACE_BLOCK)
			{
				RenderTile(x, y, 1, 0, 77);
			}
			else if (g_mapInfo.pMapPointer[y][x] == ENUM_SPACE_BOX_POINT)
			{
				RenderTile(x, y, 1, 0, 81);
			}
		}
	}

	ubox_enable_screen();
}

void DrawBox()
{

	for (int y = 1; y < SOCOBAN_MAX_HEIGHT - 1; y++)
	{
		for (int x = 1; x < SOCOBAN_MAX_WIDTH - 1; x++)
		{
			if (g_mapInfo.pMapPointer[y][x] == ENUM_SPACE_BOX_POINT && g_mapInfo.boxInfo[y][x] != g_mapInfo.pMapPointer[y][x])
			{
				RenderTile(x, y, 1, 0, 81);
			}

			if (g_mapInfo.boxInfo[y][x] == ENUM_SPACE_BOX)
			{
				RenderTile(x, y, 1, 0, 79);
			}
		}
	}
}

void DrawPlayer()
{
	RenderTile(g_playerInfo.x, g_playerInfo.y, 1, 0, 128);

	if (g_playerInfo.old_x == 0 && g_playerInfo.old_y == 0)
		return;

	if (g_mapInfo.pMapPointer[g_playerInfo.old_y][g_playerInfo.old_x] != ENUM_SPACE_BOX_POINT)
		RenderTile(g_playerInfo.old_x, g_playerInfo.old_y, 1, 0, 85);
}

void DrawWorld()
{
	if (g_playerInfo.old_x != g_playerInfo.x || g_playerInfo.old_y != g_playerInfo.y)
	{
		DrawBox();

		DrawPlayer();

		g_playerInfo.old_x = g_playerInfo.x;
		g_playerInfo.old_y = g_playerInfo.y;
	}
}

void ProcessLogic()
{
	ProcessInput();

	UpdateGameStatus();

	DrawWorld();
}