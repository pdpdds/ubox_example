#ifndef _GAME_H
#define _GAME_H

#define WHITESPACE_TILE  129
#define BLACK_TILE 	     85
#define GREEN_TILE 	     74
#define YELLOW_TILE      77
#define WHITE_TILE 	     81

enum GAME_STATE
{
    STATE_TITLE = 0,
    STATE_IN_GAME,
    STATE_GAME_OVER,
};

void run_game();

#endif