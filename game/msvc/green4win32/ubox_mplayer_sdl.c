#include "mplayer.h"

uint8_t SONG[] = { 0,0,0,0,0 };
uint8_t EFFECTS[] = { 0,0,0,0,0 };

void mplayer_init(uint8_t* song, uint8_t sub_song)
{
	/*
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not be initialized.\n");	
	}*/
}

void mplayer_init_effects(uint8_t* effects)
{

}

void mplayer_play()
{

}

void mplayer_stop()
{

}


void mplayer_play_effect(uint8_t effect_no, uint8_t chan, uint8_t inv_vol)
{

}


void mplayer_play_effect_p(uint8_t effect_no, uint8_t chan, uint8_t inv_vol)
{

}

void mplayer_stop_effect_channel(uint8_t chan)
{

}

uint8_t mplayer_is_sound_effect_on(uint8_t chan)
{
	return 0;
}
