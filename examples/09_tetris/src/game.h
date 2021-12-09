#ifndef _GAME_H
#define _GAME_H

enum GAME_STATE
{
    STATE_TITLE = 0,
    STATE_IN_GAME,
    STATE_GAME_OVER,
};

void run_game();

#endif