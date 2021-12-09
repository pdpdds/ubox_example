#ifndef _GAME_H
#define _GAME_H

#define WHITESPACE_TILE  129
#define BLACK_TILE 	     85
#define GREEN_TILE 	     74
#define YELLOW_TILE      77
#define WHITE_TILE 	     81
#define RED_TILE 	     80

#define MAX_STAGE_NUM 5

enum GAME_STATE
{
    STATE_TITLE = 0,
    STATE_IN_GAME,
    STATE_GAME_RESET,
    STATE_NO_MAP,
    STATE_GAME_CLEAR,
};

void run_game();

#endif