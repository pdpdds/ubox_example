#include <stdint.h>
#include "ubox.h"
#include "game.h"
#include "spman.h"
#include "util.h"

#define BRICKS_X_COUNT 10
#define BRICKS_Y_COUNT 5

#define BRICK_WIDTH 24 
#define BRICK_HEIGHT 16
#define BRICK_GAP 1

#define PADDLE_WIDTH 32
#define PADDLE_HEIGHT 16

#define BALL_VELOCITY_X 2
#define BALL_VELOCITY_Y 2

struct BallInfo
{
	int vel_x, vel_y;
	int x, y;
	uint8_t radius;
};


uint8_t g_pat_base = 0;		


struct BallInfo g_ball;

int g_screen_width = 0;
int g_screen_height = 0;

int g_paddle_posx = 0;
int g_paddle_posy = 0;

char g_bricks[BRICKS_X_COUNT][BRICKS_Y_COUNT];
char g_bricks_dirty_map[BRICKS_X_COUNT][BRICKS_Y_COUNT];
extern uint8_t g_gamestate;
struct sprite_attr sp;

const unsigned char breakout_sprite[3][32] = {
{
0x00, 0x00, 0x00, 0x07, 0x0f, 0x0f, 0x1f, 0x1f,
0x1f, 0x1f, 0x0f, 0x0f, 0x03, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0,
0xf0, 0xf0, 0xe0, 0xe0, 0x80, 0x00, 0x00, 0x00,
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
}
};

void InitGame(int screen_width, int screen_height);
void ProcessLogic(int mouse_posx);
void CheckGameEnd();
void DrawWorld();
void ProcessBall() ;

void draw_map()
{
	//벽돌을 화면에 그린다.
	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			if (g_bricks[x][y] != 0)
			{
				RenderBlock(x * 3, y * 2);
			}
		}
	}
}

void run_game()
{
	g_gamestate = STATE_IN_GAME;

	spman_init();

	InitGame(32 * 8, 21 * 8);
	
	g_pat_base = spman_alloc_pat(0, breakout_sprite[0], 3, 0);
	
	ubox_disable_screen();

	ubox_fill_screen(WHITESPACE_TILE);

	draw_map();

	ubox_enable_screen();

	while (1)
	{
		if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
			break;

		ProcessLogic(0);

		DrawWorld();

		ubox_wait();

		spman_update();

		if (g_gamestate == STATE_GAME_OVER || g_gamestate == STATE_GAME_CLEAR)
			break;
	}

	spman_hide_all_sprites();
}

void ResetStage()
{
	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			g_bricks[x][y] = 1;
			g_bricks_dirty_map[x][y] = 0;
		}
	}
}

static int GetBrickCount()
{
	int remainBlocksCount = 0;

	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			if (g_bricks[x][y] != 0)
			{
				remainBlocksCount++;
			}
		}
	}

	return remainBlocksCount;
}

void ResetBall()
{
	g_ball.x = g_screen_width / 2 + 8;
	g_ball.y = g_screen_height / 2 + 8;
	g_ball.vel_x = BALL_VELOCITY_X;
	g_ball.vel_y = BALL_VELOCITY_Y;
	g_ball.radius = 4;
}



void ProcessInput(int paddle_posx)
{
	if (ubox_read_keys(8) == UBOX_MSX_KEY_LEFT)
			g_paddle_posx -= 4;

	if (ubox_read_keys(8) == UBOX_MSX_KEY_RIGHT)
			g_paddle_posx += 4;

	//패들이 오른쪽 가장자리를 넘어가지 못하게 처리한다.
	if (g_paddle_posx >= g_screen_width - PADDLE_WIDTH)
		g_paddle_posx = g_screen_width - PADDLE_WIDTH;

	if (g_paddle_posx <= 0)
		g_paddle_posx = 0;
}

//공이 패들에 맞은 부위에 따라 속도를 다르게 설정
void RandomBallSpeed()
{
	if (g_ball.x < g_paddle_posx + (PADDLE_WIDTH / 4))
		g_ball.vel_x = -3;
	else if (g_ball.x < g_paddle_posx + (PADDLE_WIDTH / 2))
		g_ball.vel_x = -2;
	else if (g_ball.x < g_paddle_posx + (PADDLE_WIDTH / 2) + (PADDLE_WIDTH / 4))
		g_ball.vel_x = 2;
	else
		g_ball.vel_x = 3;
}

void ProcessBall()
{
	//패들과 공의 충돌체크
	//공이 아래로 내려가는 경우에만 패들과 충돌할 수 있다.
	struct Rect rect;
	rect.x1 = g_paddle_posx;
	rect.y1 = g_paddle_posy;
	rect.x2 = g_paddle_posx + PADDLE_WIDTH;
	rect.y2 = g_paddle_posy + PADDLE_HEIGHT;

	if (g_ball.vel_y > 0)
	{
		//패들과 공이 충돌했다면 반전시킨다.
		if (InRect(g_ball.x, g_ball.y + g_ball.radius, &rect) == 1) 
		{			
			g_ball.vel_y *= -1; 
			RandomBallSpeed();
		}	
	}

	// 속도 기반으로 볼의 위치를 갱신한다.
	g_ball.x += g_ball.vel_x;
	g_ball.y += g_ball.vel_y;

	// 공이 화면의 가장자리와 부딪치면 속도를 반전시킨다.
	if (g_ball.x + 8 >= g_screen_width - 8)
	{
		g_ball.vel_x *= -1;
	}

	if ((g_ball.x - 8) <= 0)
	{
		g_ball.vel_x *= -1;
	}

	if (g_ball.y - 8 <= 0) {
		g_ball.vel_y *= -1;
	}

	//벽돌 상태를 갱신한다
	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			if (g_bricks[x][y] != 0) //벽돌이 존재한다면
			{
				//벽돌과 공이 충돌했다면 공의 y속도를 반전시키고 벽돌을 없앤다.

				struct Rect rect;
				rect.x1 = x * BRICK_WIDTH;
				rect.y1 = y * BRICK_HEIGHT;
				rect.x2 = (x + 1) * BRICK_WIDTH - BRICK_GAP;
				rect.y2 = (y + 1) * BRICK_HEIGHT - BRICK_GAP;

				char result = 0;
				if (g_ball.vel_y > 0)
				{
					result = InRect(g_ball.x, g_ball.y + g_ball.radius, &rect);
				}

				if (g_ball.vel_y < 0)
				{
					result = InRect(g_ball.x, g_ball.y - g_ball.radius, &rect);
				}

				if (result)
				{
					g_ball.vel_y *= -1;
					g_bricks[x][y] = 0;
					g_bricks_dirty_map[x][y] = 1;
				}
			}
		}
	}
}

void CheckGameEnd()
{
	if (GetBrickCount() == 0)
		g_gamestate = STATE_GAME_CLEAR;

	// 공이 바닥으로 내려가면 업데이트를 멈추고 대기상태로 만든다.
	if (g_ball.y >= g_screen_height)
		g_gamestate = STATE_GAME_OVER;
}

void InitGame(int screen_width, int screen_height)
{
	g_screen_width = screen_width;
	g_screen_height = screen_height;

	g_paddle_posy = screen_height - PADDLE_HEIGHT;
	g_paddle_posx = g_screen_width / 2 - PADDLE_WIDTH;

	ResetStage();
	ResetBall();

	
}

void ProcessLogic(int mouse_posx)
{
	ProcessInput(mouse_posx);
	ProcessBall();
	CheckGameEnd();
}

void DrawWorld()
{
	//벽돌을 화면에서 지운다.
	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			if (g_bricks_dirty_map[x][y] == 1)
			{
				g_bricks_dirty_map[x][y] = 0;
				EraseTiles(x * 3, y * 2, 3, 2, BLACK_TILE);
			}
		}
	}	

	//공을 화면에 그린다
	sp.x = g_ball.x - 8;
	sp.y = g_ball.y - 8;
	sp.pattern = g_pat_base + 0 * 8;
	// green
	sp.attr = 13;
	spman_alloc_sprite(&sp);

	// 패들을 화면에 그린다.	
	sp.x = g_paddle_posx;
	sp.y = g_paddle_posy;
	sp.pattern = g_pat_base + 1 * 8;
	// 녹색
	sp.attr = 15;
	spman_alloc_sprite(&sp);

	sp.x = g_paddle_posx + PADDLE_WIDTH / 2;
	sp.y = g_paddle_posy;
	sp.pattern = g_pat_base + 1 * 8;
	// 녹색
	sp.attr = 11;
	spman_alloc_sprite(&sp);
}