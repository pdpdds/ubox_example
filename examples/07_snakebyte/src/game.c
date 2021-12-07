#include "ubox.h"
#include "game.h"
#include "util.h"
#include <string.h>

//256 * 168 화면 생성
#define TILE_WIDTH 8
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

struct Rect
{
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
};

typedef enum
{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN
} eDirection;

typedef enum
{
	NOTHING = 0,
	FROG,
	SNAKE
} eObject;

typedef struct SnakeNode
{
	int dir;
	char x;
	char y;
	struct SnakeNode *next;
} SnakeNode;

int g_score = 0; //획득점수
char g_objMap[MAP_HEIGHT][MAP_WIDTH];

extern uint8_t g_gamestate;
int step_time = 5;		 //스네이크 이동 시간 오프셋
int input_step_time = 4; //입력 수용 오프셋

long g_next_step = 0;		//조각 이동가능한 시간. 현재시간이 이 값을 넘으면 조각을 이동시킨다.
long g_next_input_step = 0; //입력받을 수 있는 시간. 현재시간이 이 값을 넘지 못하면 입력을 받을 수 없다.

SnakeNode *g_player = 0;

void InitGame();
void ProcessLogic(SnakeNode *player);
void DrawObject();
void GenerateNewFrog();
void DrawBackground();

void run_game()
{
	g_gamestate = STATE_IN_GAME;

	InitGame();
	ubox_disable_screen();
	ubox_fill_screen(WHITESPACE_TILE);
	DrawBackground();
	ubox_enable_screen();

	while (1)
	{
		if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
			break;

		ProcessLogic(g_player);

		DrawObject();

		ubox_wait();

		if (g_gamestate == STATE_GAME_OVER)
			break;
	}
}

void InitGame()
{
	srand(now());
	//srand(time(NULL));

	g_player = malloc(sizeof(SnakeNode));
	g_player->dir = rand() % 4; //초기 방향

	g_player->x = MAP_WIDTH / 2;
	g_player->y = MAP_HEIGHT / 2;
	g_player->next = NULL;

	int i, j;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			g_objMap[i][j] = NOTHING;
		}
	}

	GenerateNewFrog();

	g_next_step = now() + step_time;
	g_next_input_step = now() + input_step_time;
}

SnakeNode* MoveBody(SnakeNode *node, int xPos, int yPos) 
{
    if (node->next == NULL) 
	{
        g_objMap[node->y][ node->x] = NOTHING;
    }
    else 
	{
        node->next = MoveBody(node->next, node->x, node->y);
    }
    node->x = xPos;
    node->y = yPos;
    return node;
}


void GenerateNewFrog()
{
	char randy = 0, randx = 0;

	do
	{
		randy = (int)(rand() % MAP_HEIGHT);
		randx = (int)(rand() % MAP_WIDTH);

	} while (g_objMap[randy][randx] != NOTHING);
	g_objMap[randy][randx] = FROG;
}

char CheckWall(SnakeNode *player)
{	
	char result = 1;
	if (player->dir == LEFT)
	{
		if (player->x > 0)		
			result = 0;				
	}
	else if (player->dir == RIGHT)
	{
		if (player->x < MAP_WIDTH - 1)
			result = 0;		
	}
	else if (player->dir == UP)
	{
		if (player->y > 0)		
			result = 0;		
	}
	else if (player->dir == DOWN)
	{
		if (player->y < MAP_HEIGHT - 1)			
			result = 0;		
	}

	return result;
}

void ProcessLogic(SnakeNode *player)
{
	if (g_gamestate != STATE_IN_GAME)
		return;

	//키 입력을 받을 수 있는 시간인가
	if (now() > g_next_input_step)
	{
		//다음 키 입력을 받을 시간을 갱신
		g_next_input_step = now() + input_step_time;
		char x_offset = 0;
		char y_offset = 0;

		switch(ubox_read_keys(8))
		{
		case UBOX_MSX_KEY_LEFT:
			player->dir = LEFT;
			break;
		case UBOX_MSX_KEY_RIGHT:
			player->dir = RIGHT;
			break;
		case UBOX_MSX_KEY_UP:
			player->dir = UP;
			break;
		case UBOX_MSX_KEY_DOWN:
			player->dir = DOWN;
			break;
		}

		switch(player->dir)
		{
		case LEFT:
			x_offset = -1;
			break;
		case RIGHT:			
			x_offset = 1;
			break;
		case UP:			
			y_offset = -1;
			break;
		case DOWN:			
			y_offset = 1;
			break;
		}

		if (now() > g_next_step)
		{		
			if(CheckWall(player))
			{
				g_gamestate = STATE_GAME_OVER;
				return;
			}

			char next_pos_x = player->x + x_offset;
			char next_pos_y = player->y + y_offset;

			char objectType = g_objMap[next_pos_y][next_pos_x];

			switch (objectType)
			{
			case FROG:			
				g_score++;
				SnakeNode *tail = player; //뱀의 마지막 부분을 찾는다
				while (tail->next != NULL)
				{
					tail = tail->next;
				}
				int new_node_x = tail->x;
				int new_node_y = tail->y;

				MoveBody(player, next_pos_x, next_pos_y);
				
				SnakeNode *newNode = malloc(sizeof(SnakeNode));
				newNode->x = new_node_x;
				newNode->y = new_node_y;

				newNode->dir = tail->dir;
				newNode->next = NULL;
				tail->next = newNode;

				GenerateNewFrog();			
				break;
			case SNAKE:
				g_gamestate = STATE_GAME_OVER;
				break;
			default:
				MoveBody(player, next_pos_x, next_pos_y);
				break;
			}

			g_objMap[next_pos_y][next_pos_x] = SNAKE;
			g_next_step = now() + step_time;
		}
	}
}

void DrawObject()
{
	uint8_t c = 0;

	//맵의 정보를 그린다.
	for (uint8_t row = 0; row < MAP_HEIGHT; row++)
	{
		for (uint8_t col = 0; col < MAP_WIDTH; col++)
		{
			if (g_objMap[row][col] == NOTHING) //정보가 없으면 검은색을 그린다.
				c = BLACK_TILE;

			else if (g_objMap[row][col] == FROG) //개구리라면 녹색을 그린다.
				c = GREEN_TILE;

			else if (g_objMap[row][col] == SNAKE)
			{
				//뱀의 머리라면 노란색을 그리고 그렇지 않으면 흰색을 그린다.
				if (g_player->x == col && g_player->y == row)
					c = YELLOW_TILE;
				else
					c = WHITE_TILE;
			}

			RenderTile(col + 1, row + 1, c); //타일을 그린다.
		}
	}
}

void DrawBackground()
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
