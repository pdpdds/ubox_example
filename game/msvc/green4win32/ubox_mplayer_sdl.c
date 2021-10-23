#include "mplayer.h"
#include "SDL2/SDL_mixer.h"
#include "main.h"

uint8_t SONG[] = { 0,0,0,0,0 };
uint8_t EFFECTS[] = { 0,0,0,0,0 };

Mix_Chunk* ingame = 0;
Mix_Chunk* gameover = 0;

void mplayer_init(uint8_t* song, uint8_t sub_song)
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
	{
		printf("SDL_mixer could not be initialized.\n");	
	}

	if (SONG_IN_GAME == sub_song)
	{
		if(!ingame)
			ingame = Mix_LoadWAV("ingame.wav");

		if (Mix_PlayChannel(-1, ingame, -1) == -1)
		{
			return;
		}
	}
	else if (SONG_GAME_OVER == sub_song)
	{
		if (!gameover)
			gameover = Mix_LoadWAV("gameover.wav");

		if (Mix_PlayChannel(-1, gameover, 0) == -1)
		{
			return;
		}
	}
	else if (SONG_SILENCE == sub_song)
	{
		Mix_HaltChannel(-1);
	}


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
