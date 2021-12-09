#include "ubox.h"
#include "game.h"
#include "util.h"

#define SOCOBAN_MAX_WIDTH 20
#define SOCOBAN_MAX_HEIGHT 20

enum eSpaceProperty
{
	SPACE_EMPTY,
	SPACE_BLOCK,
	SPACE_BOX_POINT,
	SPACE_BOX,
};

struct PlayerInfo
{
	uint8_t x;
	uint8_t y;
	uint8_t prev_x;
	uint8_t prev_y;
};

struct MapInfo
{
	uint8_t stageNum;
	uint8_t boxCount;
	unsigned char mapData[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT];
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

const unsigned char (*g_stages[MAX_STAGE_NUM])[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT] = {
	&g_map1,
	&g_map2,
	&g_map3,
	&g_map4,
	&g_map5,
};	

struct MapInfo g_mapInfo = {0, 0};
struct PlayerInfo g_playerInfo;
extern uint8_t g_gamestate;

void InitGame();
void ProcessLogic();
void DrawMap();
void DrawBoard();

void run_game()
{
	g_gamestate = STATE_IN_GAME;

	if (g_mapInfo.stageNum >= MAX_STAGE_NUM)
	{
		g_gamestate = STATE_NO_MAP;
		g_mapInfo.stageNum = 0;
		return;
	}

	InitGame();

	ubox_disable_screen();
	ubox_fill_screen(WHITESPACE_TILE);
	DrawBoard();
	ubox_enable_screen();

	while (1)
	{
		if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
			break;

		ProcessLogic();

		DrawMap();

		ubox_wait();

		if (g_gamestate == STATE_GAME_CLEAR ||
			g_gamestate == STATE_GAME_RESET)
			break;
	}
}

void DrawBoard()
{
	for (int index = 0; index < SOCOBAN_MAX_WIDTH; index++)
		RenderTile(index, 0, GREEN_TILE);

	for (int index = 0; index < SOCOBAN_MAX_WIDTH; index++)
		RenderTile(index, SOCOBAN_MAX_HEIGHT - 1, GREEN_TILE);

	for (int index = 1; index < SOCOBAN_MAX_HEIGHT - 1; index++)
		RenderTile(0, index, GREEN_TILE);

	for (int index = 0; index < SOCOBAN_MAX_HEIGHT - 1; index++)
		RenderTile(SOCOBAN_MAX_WIDTH - 1, index, GREEN_TILE);
}

void SetupPlayerPosition()
{
	g_playerInfo.x = 7;
	g_playerInfo.y = 11;

	if (g_mapInfo.stageNum == 3 || 
	    g_mapInfo.stageNum == 4)
	{
		g_playerInfo.x = 7;
		g_playerInfo.y = 9;
	}
	
	g_playerInfo.prev_x = 0;
	g_playerInfo.prev_y = 0;
}

void SetupWorld()
{
	for (int y = 0; y < SOCOBAN_MAX_HEIGHT; y++)
	{
		for (int x = 0; x < SOCOBAN_MAX_WIDTH; x++)
		{
			if (g_mapInfo.mapData[y][x] == SPACE_BOX)
			{
				g_mapInfo.boxInfo[y][x] = g_mapInfo.mapData[y][x];
				g_mapInfo.boxCount++;
			}
			else
			{
				g_mapInfo.boxInfo[y][x] = SPACE_EMPTY;
			}
		}
	}
}

void InitGame()
{
	for (int x = 0; x < SOCOBAN_MAX_WIDTH; x++)
		for (int y = 0; y < SOCOBAN_MAX_HEIGHT; y++)
			g_mapInfo.mapData[x][y] = (*g_stages[g_mapInfo.stageNum])[x][y];

	SetupPlayerPosition();
	
	SetupWorld();
}

char IsCanGo(const char xdir, const char ydir)
{
	uint8_t xPos = g_playerInfo.x;
	uint8_t yPos = g_playerInfo.y;

	if (g_mapInfo.mapData[yPos][xPos] == SPACE_BLOCK)
		return 0;

	if (g_mapInfo.boxInfo[yPos][xPos] == SPACE_BOX)
	{
		if (g_mapInfo.mapData[yPos + ydir][xPos + xdir] == SPACE_BLOCK ||
			g_mapInfo.boxInfo[yPos + ydir][xPos + xdir] == SPACE_BOX)
			return 0;
	}

	return 1;
}

char ProcessMove(char xdir, char ydir)
{
	if (xdir == 0 && ydir == 0)
		return 0;

	g_playerInfo.prev_x = g_playerInfo.x;
	g_playerInfo.prev_y = g_playerInfo.y;

	g_playerInfo.x += xdir;
	g_playerInfo.y += ydir;

	if (!IsCanGo(xdir, ydir))
	{
		g_playerInfo.x -= xdir;
		g_playerInfo.y -= ydir;
		return 0;
	}

	uint8_t xPos = g_playerInfo.x;
	uint8_t yPos = g_playerInfo.y;
	
	if (g_mapInfo.boxInfo[yPos][xPos] == SPACE_BOX) 
	{
		g_mapInfo.boxInfo[yPos][xPos] = SPACE_EMPTY;
		g_mapInfo.boxInfo[yPos + ydir][xPos + xdir] = SPACE_BOX;
	}

	return 1;
}

void ProcessInput()
{
	if (ubox_read_keys(8) == UBOX_MSX_KEY_LEFT)
		ProcessMove(-1, 0);

	if (ubox_read_keys(8) == UBOX_MSX_KEY_RIGHT)
		ProcessMove(1, 0);

	if (ubox_read_keys(8) == UBOX_MSX_KEY_UP)
		ProcessMove(0, -1);

	if (ubox_read_keys(8) == UBOX_MSX_KEY_DOWN)
		ProcessMove(0, 1);

	if (ubox_read_keys(4) == UBOX_MSX_KEY_P)
	{
		g_gamestate = STATE_GAME_RESET;
	}
}

void UpdateGameStatus()
{
	int iCurrentExactBoxCount = 0;

	for (int y = 0; y < SOCOBAN_MAX_HEIGHT; y++)
		for (int x = 0; x < SOCOBAN_MAX_WIDTH; x++)
		{
			if (g_mapInfo.boxInfo[y][x] == SPACE_BOX && g_mapInfo.mapData[y][x] == SPACE_BOX_POINT)
				iCurrentExactBoxCount++;
		}

	if (iCurrentExactBoxCount == g_mapInfo.boxCount)
	{
		g_mapInfo.stageNum++;
		g_mapInfo.boxCount = 0;
		g_gamestate = STATE_GAME_CLEAR;
	}
}

void DrawObject()
{
	for (int y = 1; y < SOCOBAN_MAX_HEIGHT - 1; y++)
	{
		for (int x = 1; x < SOCOBAN_MAX_WIDTH - 1; x++)
		{
			if (g_mapInfo.mapData[y][x] == SPACE_BLOCK)
				RenderTile(x, y, YELLOW_TILE);

			if (g_mapInfo.mapData[y][x] == SPACE_BOX_POINT)
				if(g_mapInfo.boxInfo[y][x] != g_mapInfo.mapData[y][x])
					RenderTile(x, y, WHITE_TILE);	

			if (g_mapInfo.boxInfo[y][x] == SPACE_BOX)
				RenderTile(x, y, GREEN_TILE);
		}
	}
}

void DrawPlayer()
{
	RenderTile(g_playerInfo.x, g_playerInfo.y, RED_TILE);

	if (g_playerInfo.prev_x == 0 && g_playerInfo.prev_y == 0)
		return;

	if (g_mapInfo.mapData[g_playerInfo.prev_y][g_playerInfo.prev_x] != SPACE_BOX_POINT)
		RenderTile(g_playerInfo.prev_x, g_playerInfo.prev_y, BLACK_TILE);
}

void DrawMap()
{
	if (g_playerInfo.prev_x != g_playerInfo.x || g_playerInfo.prev_y != g_playerInfo.y)
	{
		DrawObject();
		DrawPlayer();

		g_playerInfo.prev_x = g_playerInfo.x;
		g_playerInfo.prev_y = g_playerInfo.y;
	}
}

void ProcessLogic()
{
	ProcessInput();
	UpdateGameStatus();
	DrawMap();
}