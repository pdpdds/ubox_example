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
void DrawWorld();
void GenerateNewFrog();

extern void RenderTile(int x, int y, int tileNum);

#endif