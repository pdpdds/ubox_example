#ifndef _GAME_H
#define _GAME_H

enum GAME_STATE
{
    STATE_TITLE = 0,
    STATE_IN_GAME,
    STATE_GAME_OVER,
};


void run_game();
void InitGame(int screen_width, int screen_height);
void ProcessLogic(int mouse_posx);
void CheckGameEnd();
void DrawWorld();
void UpdateBall() ;

extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b, int fill); //사각형을 그리는 함수
extern void RenderCircle(int x, int y, int radius, int r, int g, int b); // 원을 그리는 함수
extern void RenderLine(int x, int y, int x2, int y2, int r, int g, int b);
extern void RenderText(int x, int y, char* szMessage);
extern void RenderTile(int x, int y, int x_count, int y_count, int tileNum);
extern void EraseTile(int x, int y, int x_count, int y_count, int tileNum);

#endif