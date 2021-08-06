#include <stdint.h>
#include "ubox.h"
#include "game.h"
#include "spman.h"

#define WHITESPACE_TILE 129

#define BRICKS_X_COUNT 10
#define BRICKS_Y_COUNT 5

#define BRICK_WIDTH 24 
#define BRICK_HEIGHT 16
#define BRICK_GAP 1

#define PADDLE_WIDTH 32
#define PADDLE_HEIGHT 16

#define BALL_VELOCITY_X 1
#define BALL_VELOCITY_Y 1

struct BallInfo
{
	int vel_x, vel_y;
	uint8_t x, y;
	uint8_t radius;
};

struct entity
{
	uint8_t type;
	uint8_t x;
	uint8_t y;
	uint8_t dir;
	uint8_t pat;
	uint8_t flags;
	uint8_t delay;
	uint8_t frame;
	void (*update)();
};

struct entity sample;

struct BallInfo g_ball;

int g_screen_width = 0;
int g_screen_height = 0;

int g_paddle_posx = 0;
int g_paddle_posy = 0;

char g_bricks[BRICKS_X_COUNT][BRICKS_Y_COUNT];
char g_bricks_dirty_map[BRICKS_X_COUNT][BRICKS_Y_COUNT];
extern uint8_t g_gamestate;

const unsigned char enemy_sprite[3][32] = {
	{
		0x00,
		0x00,
		0x1f,
		0x3f,
		0x3f,
		0x7f,
		0x7f,
		0x7f,
		0x7f,
		0x3f,
		0x3f,
		0x0e,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x80,
		0x80,
		0xc0,
		0xc0,
		0xc0,
		0xc0,
		0x80,
		0x80,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
	},
	{
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
	}};

struct sprite_attr sp;

void draw_map()
{
	//벽돌을 화면에 그린다.
	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			if (g_bricks[x][y] != 0)
			{
				RenderTile(x * 3, y * 2, 3, 2, 7);
			}
		}
	}
}

void run_game()
{
	g_gamestate = STATE_IN_GAME;

	InitGame(32 * 8, 21 * 8);

	spman_init();

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

		if (g_gamestate == STATE_GAME_OVER)
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
	g_ball.x = g_screen_width / 2;
	g_ball.y = g_screen_height / 2;
	g_ball.vel_x = BALL_VELOCITY_X;
	g_ball.vel_y = BALL_VELOCITY_Y;
	g_ball.radius = 10;
}

int InRect(int posx, int posy, int left, int top, int right, int bottom)
{
	if (posx > left && posx < right && posy > top && posy < bottom)
		return 1;

	return 0;
}

void UpdatePaddle(int paddle_posx)
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

//공이 패들에 맞는 위치와 패들의 이동속도를 사용해서 공의 속도를 갱신하자
//지금은 구현하지 않음
void RandomBallSpeed()
{
	if (g_ball.x < g_paddle_posx + (PADDLE_WIDTH / 2))
	{
		if (g_ball.vel_x > 0)
			g_ball.vel_x *= -1;
	}
	else
	{
		if (g_ball.vel_x < 0)
			g_ball.vel_x *= -1;
	}
}

void UpdateBall()
{
	//패들과 공의 충돌체크
	//공이 아래로 내려가는 경우에만 패들과 충돌할 수 있다.
	if (g_ball.vel_y > 0)
	{
		if (InRect(g_ball.x + g_ball.radius, g_ball.y + g_ball.radius,
			g_paddle_posx,
			g_paddle_posy,
			g_paddle_posx + (PADDLE_WIDTH),
			g_paddle_posy + PADDLE_HEIGHT) == 1) 
		{			
			g_ball.vel_y *= -1; //패들과 공이 충돌했다면 반전시킨다.

			RandomBallSpeed();
		}	
	}

	// 속도 기반으로 볼의 위치를 갱신한다.
	g_ball.x += g_ball.vel_x;
	g_ball.y += g_ball.vel_y;

	//벽돌 상태를 갱신한다
	for (int y = 0; y < BRICKS_Y_COUNT; y++) 
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++) 
		{
			if (g_bricks[x][y] != 0)  //벽돌이 존재한다면
			{ 
				//벽돌과 공이 충돌했다면 공의 y속도를 반전시키고 벽돌을 없앤다.
				if (InRect(g_ball.x, g_ball.y,
					x * BRICK_WIDTH, y * BRICK_HEIGHT,
					(x + 1) * BRICK_WIDTH - BRICK_GAP,
					(y + 1) * BRICK_HEIGHT - BRICK_GAP) == 1) 
				{					
					g_ball.vel_y *= -1;
					g_bricks[x][y] = 0;
					g_bricks_dirty_map[x][y] = 1;
				}
			}
		}
	}

	// 공이 화면의 가장자리와 부딪치면 속도를 반전시킨다.
	if (g_ball.x >= g_screen_width - 16)
	{
		g_ball.vel_x *= -1;
	}
	if (g_ball.x == 0)
	{
		g_ball.vel_x *= -1;
	}

	if (g_ball.y == 0) {
		g_ball.vel_y *= -1;
	}

	// 공이 바닥으로 내려가면 업데이트를 멈추고 대기상태로 만든다.
	if (g_ball.y >= g_screen_height)
	{
		g_gamestate = STATE_GAME_OVER;
	}
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
	UpdatePaddle(mouse_posx);

	switch (g_gamestate)
	{
	case STATE_IN_GAME:
	{
		UpdateBall();
	}
	break;
	/*case GAME_WAIT:
	{
		if (0)
		{

			ResetBall();
			g_gamestate = GAME_PLAY;
		}
	}*/
	break;
	}
}

void CheckGameEnd()
{
	if (GetBrickCount() == 0)
	{
		InitGame(g_screen_width, g_screen_height);
	}
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
				EraseTile(x * 3, y * 2, 3, 2, 110);
			}
		}
	}

	sample.pat = spman_alloc_pat(0, enemy_sprite[0], 6, 0);

	if (g_gamestate == STATE_IN_GAME)
	{
		//공을 화면에 그린다
		//RenderCircle(g_ball.x, g_ball.y, 4, 255, 255, 0);
		sp.x = g_ball.x;
		sp.y = g_ball.y;
		sp.pattern = sample.pat + 0 * 8;
		// green
		sp.attr = 12;
		spman_alloc_fixed_sprite(&sp);
	}

	// 패들을 화면에 그린다.
	//RenderRect(g_paddle_posx, g_paddle_posy, g_paddle_posx + PADDLE_WIDTH, g_paddle_posy + PADDLE_HEIGHT, 255, 255, 255, 1);

	sample.pat = spman_alloc_pat(0, enemy_sprite[0], 6, 0);
	sp.x = g_paddle_posx;
	sp.y = g_paddle_posy;
	sp.pattern = sample.pat + 1 * 8;
	// green
	sp.attr = 15;
	spman_alloc_fixed_sprite(&sp);

	sp.x = g_paddle_posx + PADDLE_WIDTH / 2;
	sp.y = g_paddle_posy;
	sp.pattern = sample.pat + 1 * 8;
	// green
	sp.attr = 15;
	spman_alloc_fixed_sprite(&sp);
}