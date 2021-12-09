#include <stdlib.h>
#include "ubox.h"
#include "game.h"
#include "util.h"
#include <string.h>

#define GAME_WIDTH 256
#define GAME_HEIGHT 128

//140 * 196 화면 생성
//타일크기 8
#define ROWS 18
#define COLS 10
#define TILE_WIDTH 8

enum eSTEP
{
	COLLIDE_NO = 0,
	COLLIDE,
	COLLIDE_GAME_OVER,
};

//조각. 총 7개
const uint8_t shape_bar[4][4] = {
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0}};

const uint8_t shape_square[4][4] = {
	{0, 0, 0, 0},
	{0, 1, 1, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0}};

const uint8_t shape_j[4][4] = {
	{0, 0, 0, 0},
	{1, 0, 0, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 0}};

const uint8_t shape_l[4][4] = {
	{0, 0, 1, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}};

const uint8_t shape_s[4][4] = {
	{0, 1, 1, 0},
	{1, 1, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}};

const uint8_t shape_z[4][4] = {
	{1, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}};

const uint8_t shape_t[4][4] = {
	{0, 1, 0, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}};

const uint8_t (*shapes[7])[4][4] = {
	&shape_bar,
	&shape_square,
	&shape_l,
	&shape_j,
	&shape_s,
	&shape_z,
	&shape_t,
};

//조각 정보 구조체
typedef struct
{
	uint8_t shape[4][4];
	uint8_t old_shape[4][4];
	uint8_t x;
	uint8_t y;
	uint8_t id;
} Piece;

//키입력 구조체
typedef struct
{
	uint8_t right;
	uint8_t left;
	uint8_t rot;
	uint8_t drop;
	uint8_t fast;
} KeyTable;

int step_time = 20;		 //조각 이동 시간 오프셋
int input_step_time = 4; //입력 수용 오프셋

long g_next_step = 0;		//조각 이동가능한 시간. 현재시간이 이 값을 넘으면 조각을 이동시킨다.
long g_next_input_step = 0; //입력받을 수 있는 시간. 현재시간이 이 값을 넘지 못하면 입력을 받을 수 없다.

Piece g_piece = {{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
				 {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
				 (COLS / 2 - 2),
				 0,
				 0};

Piece g_next_piece = {{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
					  {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
					  (COLS / 2 - 2),
					  0,
					  0};

int g_score = 0;			  //획득점수
uint8_t g_map[ROWS * COLS] = {0}; //타일공간 최초에는 모두 0으로 설정한다.
KeyTable g_key_table = {0};	  // 키 입력을 저장

extern uint8_t g_gamestate;
void ProcessGame();
char CheckGameEnd();
void DrawBoard();
void DrawNextPiece();
void InitGame();
void RenderPiece(uint8_t shape[][4], uint8_t xPos, uint8_t yPos, uint8_t color);
void DrawWorld(uint8_t collision_type, uint8_t old_x, uint8_t old_y);

uint8_t Rotate(Piece *piece)
{
	Piece tmp;
	memcpy(&tmp, piece, sizeof(tmp));

	//조각은 회전할 경우 3x4 또는 4 x 3안에 타일이 모두 들어간다.
	uint8_t n = 3;
	if (piece->id == 0)
		n = 4;
	for (uint8_t row = 0; row < n; row++)
	{
		for (uint8_t col = 0; col < n; col++)
		{
			piece->shape[row][col] = tmp.shape[n - col - 1][row];
		}
	}
	return 0;
}

//1 충돌, 0 충돌하지 않음
char IsColliding(const uint8_t *map, const Piece *piece)
{
	for (uint8_t row = 0; row < 4; row++)
	{
		for (uint8_t col = 0; col < 4; col++)
		{
			uint8_t mapx = piece->x + col;
			uint8_t mapy = piece->y + row;
			if ((piece->shape[row][col] &&
				 (map[mapy * COLS + mapx] ||
				  mapy >= ROWS || mapx >= COLS)))
			{
				return 1;
			}
		}
	}

	return 0;
}

uint8_t PlacePiece(uint8_t *map, Piece *piece)
{
	for (uint8_t row = 0; row < 4; row++)
	{
		for (uint8_t col = 0; col < 4; col++)
		{
			uint8_t mapx = piece->x + col;
			uint8_t mapy = piece->y + row;
			uint8_t tile = piece->shape[row][col];
			if (tile)
			{
				map[mapy * COLS + mapx] = tile;
			}
		}
	}
	return 0;
}

void GetPiece(Piece *piece)
{
	piece->id = rand() % 7;
	memcpy(piece->shape, shapes[piece->id], sizeof(piece->shape));
}

void DropLine(uint8_t *map, uint8_t n)
{
	for (uint8_t row = n; row > 0; row--)
	{
		if (row == ROWS)
			continue;

		memcpy(&map[row * COLS], &map[(row - 1) * COLS], sizeof(uint8_t) * COLS);
	}
}

//타일이 전부 채워진 줄을 검색해서
//보드에서 제거한다.
uint8_t ClearLines(uint8_t *map)
{
	uint8_t n_lines = 0;
	for (uint8_t row = 0; row < ROWS; row++)
	{
		uint8_t result = 1;
		for (uint8_t col = 0; col < COLS; col++)
		{
			if (!map[row * COLS + col])
			{
				result = 0;
			}
		}
		if (result)
		{
			DropLine(map, row);
			n_lines++;
		}
	}
	return n_lines;
}

//조각을 이동시킨 다음 보드의 타일과 충돌이 있다면
//몇개의 라인을 제거할 수 있는지 체크한다.
//그리고 새로운 조각을 선택하고 좌표를 초기화한다.
//새로운 사각형이 나오자마자 보드의 타일과 충돌한다면 게임이 종료됐다는 걸 의미한다.

uint8_t Step(uint8_t *map, Piece *piece, uint8_t *cleared_lines)
{
	//조각을 y축으로 1칸 이동
	piece->y += 1;
	if (!IsColliding(map, piece))
	{
		//충돌없음
		return COLLIDE_NO;
	}

	piece->y -= 1;
	PlacePiece(map, piece);
	*cleared_lines = ClearLines(map);

	return COLLIDE;
}

//한번에 없앤 라인수에 따른 점수 갱신
void UpdateScore(uint8_t clearline)
{
	switch (clearline)
	{
	case 1:
		g_score += 40;
		break;
	case 2:
		g_score += 100;
		break;
	case 3:
		g_score += 300;
		break;
	case 4:
		g_score += 1200;
		break;
	}
}

void DrawBackground()
{
	for (uint8_t index = 0; index < COLS + 1; index++)
		RenderTile(index, 0, YELLOW_TILE);

	for (uint8_t index = 0; index < COLS + 2; index++)
		RenderTile(index, ROWS + 1, YELLOW_TILE);

	for (uint8_t index = 1; index < ROWS + 1; index++)
		RenderTile(0, index, YELLOW_TILE);

	for (uint8_t index = 0; index < ROWS + 1; index++)
		RenderTile(COLS + 1, index, YELLOW_TILE);

	DrawNextPiece();
}

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

		ProcessGame();
		
		ubox_wait();

		if (g_gamestate == STATE_GAME_OVER)
			break;
	}
}

void InitGame()
{

	memset(g_map, 0, ROWS * COLS * sizeof(uint8_t));

	srand(now());

	g_next_step = now() + step_time;
	g_next_input_step = now() + input_step_time;

	GetPiece(&g_piece);

	GetPiece(&g_next_piece);
	//새 조각을 선택한다.
	g_piece.x = (COLS / 2 - 2);
	g_piece.y = 0;
	
}

void ProcessGame()
{
	uint8_t prev_piece_x = g_piece.x;
	uint8_t prev_piece_y = g_piece.y;
		
	//키 입력을 받을 수 있는 시간인가
	if (now() > g_next_input_step)
	{
		
		g_key_table.right = ubox_read_keys(8) == UBOX_MSX_KEY_RIGHT;
		g_key_table.left = ubox_read_keys(8) == UBOX_MSX_KEY_LEFT;
		g_key_table.rot = ubox_read_keys(8) == UBOX_MSX_KEY_UP;
		g_key_table.fast = ubox_read_keys(8) == UBOX_MSX_KEY_DOWN; //아래 방향키를 눌렀다면 조각이 떨어지는 속도를 높인다.
		g_key_table.drop = ubox_read_keys(8) == UBOX_MSX_KEY_SPACE;

		//다음 키 입력을 받을 시간을 갱신
		g_next_input_step = now() + input_step_time;

		//오른쪽 방향키를 눌렀다면 조각을 오른쪽으로 이동시킨다.
		//만약 보드의 타일과 충돌한다면 조각의 좌표를 원래대로 되돌린다.
		if (g_key_table.right)
		{
			g_piece.x += 1;
			if (IsColliding(g_map, &g_piece))
			{
				g_piece.x -= 1;
			}
		}

		//왼쪽 방향키를 눌렀다면 조각을 왼쪽으로 이동시킨다.
		//만약 보드의 타일과 충돌한다면 조각의 좌표를 원래대로 되돌린다.
		if (g_key_table.left)
		{
			g_piece.x -= 1;
			if (IsColliding(g_map, &g_piece))
			{
				g_piece.x += 1;
			}
		}
		//윗쪽 방향키를 눌렀다면 조각이 회전가능한지 확인하고 조각의 배열값을 갱신한다.
		if (g_key_table.rot)
		{
			Piece tempPiece;
			memcpy(&tempPiece, &g_piece, sizeof(Piece));
			Rotate(&g_piece);

			//회전시킨 조작이 보드와 충돌한다면 조각 배열을 원상태로 돌린다.
			if (IsColliding(g_map, &g_piece))
			{
				memcpy(&g_piece, &tempPiece, sizeof(Piece));
				g_key_table.rot = 0;
			}
			else
			{
				for (uint8_t row = 0; row < 4; row++)
				{
					for (uint8_t col = 0; col < 4; col++)
					{
						g_piece.old_shape[row][col] = tempPiece.shape[row][col];
					}
				}
			}

			
		}
	}

	uint8_t clear_lines = 0; //없앤 라인수

	//조각이 아래로 내려갈 수 있는지를 확인하고 보드의 타일과 충돌여부를 체크해서 게임을 계속 진행할 수 있는지 확인한다.
	uint8_t result = COLLIDE_NO;

	if (g_key_table.drop) //스페이스키를 눌러 조각을 떨어뜨리면
	{
		g_key_table.drop = 0;

		while (result == COLLIDE_NO)
		{
			result = Step(g_map, &g_piece, &clear_lines);
		}

		g_next_step = now() + step_time;
	}
	else
	{
		uint8_t accel = g_key_table.fast ? (7 * step_time / 8) : 0;
		if (now() > g_next_step - accel)
		{
				
			result = Step(g_map, &g_piece, &clear_lines);

			g_next_step = now() + step_time;
		}
	}

	if (clear_lines > 0) //없앤줄이 있으면 줄수에 따라 점수를 갱신한다.
		UpdateScore(clear_lines);

	DrawWorld(result, prev_piece_x, prev_piece_y);
}

//보드를 그린다
void DrawBoard()
{
	for (uint8_t row = 0; row < ROWS; row++)
	{
		for (uint8_t col = 0; col < COLS; col++)
		{
			uint8_t color = g_map[row * COLS + col];

			if (color != 0)
			{
				RenderTile(col + 1, row + 1, WHITE_TILE);
			}
			else
			{
				RenderTile(col + 1, row + 1, BLACK_TILE);
			}
		}
	}
}

void DrawNextPiece()
{
	for (uint8_t row = 0; row < 4; row++)
	{
		for (uint8_t col = 0; col < 4; col++)
		{
			uint8_t color = g_next_piece.shape[row][col];

			if (color != 0)
				RenderTile(col + COLS + 3, row + 2, GREEN_TILE);
			else
				RenderTile(col + COLS + 3, row + 2, BLACK_TILE);
		}
	}
}

void RenderPiece(uint8_t shape[][4], uint8_t xPos, uint8_t yPos, uint8_t color)
{
	for (uint8_t row = 0; row < 4; row++)
	{
		for (uint8_t col = 0; col < 4; col++)
		{
			if (shape[row][col])
			{
				RenderTile(col + xPos + 1, row + yPos + 1, color);
			}
		
		}
	}
}

void DrawWorld(uint8_t collision_type, uint8_t old_x, uint8_t old_y)
{
	if (g_key_table.rot)
	{
		RenderPiece(g_piece.old_shape, old_x, old_y, BLACK_TILE);
		RenderPiece(g_piece.shape, g_piece.x, g_piece.y, GREEN_TILE);
		g_key_table.rot = 0;
	}

	if (old_x != g_piece.x || old_y != g_piece.y)
	{
		RenderPiece(g_piece.shape, old_x, old_y, BLACK_TILE);
		RenderPiece(g_piece.shape, g_piece.x, g_piece.y, GREEN_TILE);
	}

	if (collision_type == COLLIDE) //충돌이 있었다면 다음에 나올 블록을 갱신한다
	{
		DrawBoard();

		memcpy(&g_piece, &g_next_piece, sizeof(Piece));
		g_piece.x = (COLS / 2 - 2);
		g_piece.y = 0;

		GetPiece(&g_next_piece);

		DrawNextPiece();

		//새로운 조각이 맵과 충돌한다면 보드가 타일로 가득찼다는 의미임
		if (IsColliding(g_map, &g_piece))
			g_gamestate = STATE_GAME_OVER;
	}
}