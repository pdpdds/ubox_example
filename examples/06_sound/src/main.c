#include <stdint.h>
#include "ubox.h"
#include "mplayer.h"

#define LOCAL
#include "tiles.h"

extern uint8_t SONG[];
extern uint8_t EFFECTS[];

#define EFX_CHAN_NO 2

enum effects
{
    EFX_NONE = 0,
    EFX_START,
    EFX_BATTERY,
    EFX_ELEVATOR,
    EFX_HIT,
    EFX_DEAD,
};

#define WHITESPACE_TILE 129

void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}

void main()
{
    
    ubox_init_isr(2);

    ubox_set_mode(2);
  
    ubox_set_colors(1, 1, 1);

    ubox_disable_screen();

    ubox_set_tiles(tiles);
    ubox_set_tiles_colors(tiles_colors);
  
    ubox_fill_screen(WHITESPACE_TILE);
	put_text(11, 11, "EFFECTS TEST");
		
    ubox_enable_screen();

    mplayer_init(SONG, 0);
	mplayer_init_effects(EFFECTS);
  
    ubox_set_user_isr(mplayer_play);

	
    while (1)
    {			
        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
            break;

		if (ubox_read_keys(0) == UBOX_MSX_KEY_1)
            mplayer_play_effect_p(EFX_START, EFX_CHAN_NO, 0);
				
		if (ubox_read_keys(0) == UBOX_MSX_KEY_2)
            mplayer_play_effect_p(EFX_BATTERY, EFX_CHAN_NO, 0);

		if (ubox_read_keys(0) == UBOX_MSX_KEY_3)
            mplayer_play_effect_p(EFX_ELEVATOR, EFX_CHAN_NO, 0);
		
		if (ubox_read_keys(0) == UBOX_MSX_KEY_4)
            mplayer_play_effect_p(EFX_HIT, EFX_CHAN_NO, 0);
		
		if (ubox_read_keys(0) == UBOX_MSX_KEY_5)
            mplayer_play_effect_p(EFX_DEAD, EFX_CHAN_NO, 0);

        ubox_wait();
    }
}
