#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#ifndef LOCAL
#define LOCAL extern
#endif

#define WHITESPACE_TILE 129

// sub-songs matching our Arkos song
// configure the song to use MSX AY
enum songs
{
    SONG_SILENCE = 0,
    SONG_IN_GAME,
    SONG_GAME_OVER,
};

// we will use 0 and 1 for the music
#define EFX_CHAN_NO 2

// sound effects matching our Arkos efx song
// configure the song to use MSX AY
enum effects
{
    EFX_NONE = 0,
    EFX_START,
    EFX_BATTERY,
    EFX_ELEVATOR,
    EFX_HIT,
    EFX_DEAD,
};

void draw_end_game();
void draw_menu();

// store the selected control
LOCAL uint8_t ctl;

// Arkos data
extern uint8_t SONG[];
extern uint8_t EFFECTS[];

#ifdef LOCAL
#undef LOCAL
#endif

#endif // _MAIN_H
