#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ubox.h"
#include "game.h"

#define WHITESPACE_TILE 129

#define GAME_WIDTH 256
#define GAME_HEIGHT 128

extern int now();

void randomize(void)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < (rand() % RAND_MAX); i++)
		(rand() % RAND_MAX);
}

void srandom(unsigned int seed)
{
	srand(seed);
}

int g_screen_width = 0;
int g_screen_height = 0;

struct Rect
{
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
};

//256 * 160 화면 생성
//타일크기 8
#define TILE_WIDTH 8
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

typedef enum
{
	left = 0,
	right,
	up,
	down
} direction;

typedef enum
{
	nothing = 0,
	frog,
	snake
} object;

typedef struct snake_node
{
	int dir;
	int x;
	int y;
	struct snake_node *next;
} snake_node;

int g_score = 0; //획득점수
int g_objMap[MAP_HEIGHT][MAP_WIDTH];

extern uint8_t g_gamestate;
int step_time = 5;		 //스네이크 이동 시간 오프셋
int input_step_time = 4; //입력 수용 오프셋

long g_next_step = 0;		//조각 이동가능한 시간. 현재시간이 이 값을 넘으면 조각을 이동시킨다.
long g_next_input_step = 0; //입력받을 수 있는 시간. 현재시간이 이 값을 넘지 못하면 입력을 받을 수 없다.

snake_node *g_player = 0;

void ProcessLogic(snake_node *player);

int GameOver()
{
	g_gamestate = STATE_GAME_OVER;
	return 0;
}

void DrawMap()
{
	for (int index = 0; index < MAP_WIDTH + 1; index++)
		RenderTile(index, 0, 77);

	for (int index = 0; index < MAP_WIDTH + 2; index++)
		RenderTile(index, MAP_HEIGHT + 1, 77);

	for (int index = 1; index < MAP_HEIGHT + 1; index++)
		RenderTile(0, index, 77);

	for (int index = 0; index < MAP_HEIGHT + 1; index++)
		RenderTile(MAP_WIDTH + 1, index, 77);
}

void run_game()
{
	g_gamestate = STATE_IN_GAME;

	InitGame(32 * 8, 21 * 8);

	ubox_disable_screen();
	ubox_fill_screen(WHITESPACE_TILE);

	DrawMap();
	ubox_enable_screen();

	while (1)
	{
		if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
			break;

		ProcessLogic(g_player);

		DrawWorld();

		ubox_wait();

		if (g_gamestate == STATE_GAME_OVER)
			break;
	}
}

void InitGame(int screen_width, int screen_height)
{
	g_screen_width = screen_width;
	g_screen_height = screen_height;

	g_player = malloc(sizeof(snake_node));
	g_player->dir = left; //초기 방향
	g_player->x = MAP_WIDTH / 2;
	g_player->y = MAP_HEIGHT / 2;
	g_player->next = NULL;

	int i, j;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			g_objMap[i][j] = nothing;
		}
	}

	srandom((unsigned int)time(NULL));
	randomize();

	GenerateNewFrog();

	g_next_step = now() + step_time;
	g_next_input_step = now() + input_step_time;
}

snake_node *move_body(snake_node *player, int tempx, int tempy)
{
	if (player->next == NULL)
	{
		g_objMap[player->y][player->x] = nothing;
	}
	else
	{
		player->next = move_body(player->next, player->x, player->y);
	}
	player->x = tempx;
	player->y = tempy;
	return player;
}

void GenerateNewFrog()
{
	int randy, randx;

	do
	{
		randy = (int)(rand() % MAP_HEIGHT);
		randx = (int)(rand() % MAP_WIDTH);
	} while (g_objMap[randy][randx] != nothing);
	g_objMap[randy][randx] = frog;
}

void ProcessLogic(snake_node *player)
{
	if (g_gamestate != STATE_IN_GAME)
		return;

	//키 입력을 받을 수 있는 시간인가
	//if (now() > g_next_input_step)
	{
		if (ubox_read_keys(8) == UBOX_MSX_KEY_LEFT)
			player->dir = left;

		if (ubox_read_keys(8) == UBOX_MSX_KEY_RIGHT)
			player->dir = right;

		if (ubox_read_keys(8) == UBOX_MSX_KEY_UP)
			player->dir = up;

		if (ubox_read_keys(8) == UBOX_MSX_KEY_DOWN)
			player->dir = down;

		//다음 키 입력을 받을 시간을 갱신
		g_next_input_step = now() + input_step_time;

		if (now() > g_next_step)
		{
			int tempx = player->x, tempy = player->y;

			if (player->dir == left)
			{

				if (player->x > 0)
				{
					tempx = player->x - 1;
				}
				else
				{
					GameOver();
				}
			}
			else if (player->dir == right)
			{

				if (player->x < MAP_WIDTH - 1)
				{
					tempx = player->x + 1;
				}
				else
				{
					GameOver();
				}
			}
			else if (player->dir == up)
			{
				if (player->y > 0)
				{
					tempy = player->y - 1;
				}
				else
				{
					GameOver();
				}
			}
			else if (player->dir == down)
			{

				if (player->y < MAP_HEIGHT - 1)
				{
					tempy = player->y + 1;
				}
				else
				{
					GameOver();
				}
			}

			if (g_objMap[tempy][tempx] == frog)
			{
				g_score++;
				snake_node *temp = player;
				while (temp->next != NULL)
				{
					temp = temp->next;
				}
				int newNodex = temp->x;
				int newNodey = temp->y;
				player = move_body(player, tempx, tempy);
				snake_node *newNode = malloc(sizeof(snake_node));
				newNode->x = newNodex;
				newNode->y = newNodey;
				newNode->dir = temp->dir;
				newNode->next = NULL;
				temp->next = newNode;

				GenerateNewFrog();
			}
			else if (g_objMap[tempy][tempx] == snake)
			{
				GameOver();
			}
			else
			{
				player = move_body(player, tempx, tempy);
			}

			g_objMap[tempy][tempx] = snake;

			g_next_step = now() + step_time;
		}
	}
}

void DrawWorld()
{
	uint8_t c = 0;

	//맵의 정보를 그린다.
	for (uint8_t row = 0; row < MAP_HEIGHT; row++)
	{
		for (uint8_t col = 0; col < MAP_WIDTH; col++)
		{

			if (g_objMap[row][col] == nothing) //정보가 없으면 검은색을 그린다.
				c = 85;

			else if (g_objMap[row][col] == frog) //개구리라면 녹색을 그린다.
				c = 74;

			else if (g_objMap[row][col] == snake)
			{
				//뱀의 머리라면 노란색을 그리고 그렇지 않으면 흰색을 그린다.
				if (g_player->x == col && g_player->y == row) 
					c = 77;
				else
					c = 81;
			}

			RenderTile(col + 1, row + 1, c); //타일을 그린다. 
		}
	}
}
