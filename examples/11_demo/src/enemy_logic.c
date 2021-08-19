#include "enemy_logic.h"
#include <stdint.h>
#include "game.h"
#include <spman.h>
#include <mplayer.h>
#include "main.h"

void update_enemy()
{
    if (self->extra == ENEMY_STATIC)
    {

        if (!gameover_delay && lives && !invuln && entities[0].x + 6 < self->x + 10 && self->x + 6 < entities[0].x + 10 && self->y == entities[0].y)
        {
            lives--;

            invuln = INVUL_TIME;

            if (!lives)
            {
                // different sound effects if is game over
                mplayer_init(SONG, SONG_SILENCE);
                mplayer_play_effect_p(EFX_DEAD, EFX_CHAN_NO, 0);
                gameover_delay = GAMEOVER_DELAY;
                g_gamestate = STATE_GAME_OVER;
            }
            else
            {
                mplayer_play_effect_p(EFX_HIT, EFX_CHAN_NO, 0);
                gameover_delay = GAMEOVER_DELAY;
                g_gamestate = STATE_GAME_RESET;
            }
        }

        sp.x = self->x;
        sp.y = self->y - 1;

        sp.pattern = self->pat;
        sp.attr = 10;
        spman_alloc_fixed_sprite(&sp);
    }

    else if (self->extra == ENEMY_MOVE)
    {
        if (!gameover_delay && lives && !invuln && entities[0].x + 6 < self->x + 10 && self->x + 6 < entities[0].x + 10 && self->y == entities[0].y)
        {
            // change direction
            self->dir ^= 1;

            // remove one life (is more like "hits")
            lives--;

            invuln = INVUL_TIME;

            if (!lives)
            {
                // different sound effects if is game over
                mplayer_init(SONG, SONG_SILENCE);
                mplayer_play_effect_p(EFX_DEAD, EFX_CHAN_NO, 0);
                gameover_delay = GAMEOVER_DELAY;
                g_gamestate = STATE_GAME_OVER;
            }
            else
            {
                mplayer_play_effect_p(EFX_HIT, EFX_CHAN_NO, 0);
                gameover_delay = GAMEOVER_DELAY;
                g_gamestate = STATE_GAME_RESET;
            }
        }

        // left or right?
        if (self->dir)
        {
            // change direction
            if (self->x == 2 || is_map_blocked(self->x, self->y + 15))
                self->dir ^= 1;
            else
                self->x -= 1;
        }
        else
        {
            // change direction
            if (self->x == 255 - 16 || is_map_blocked(self->x + 15, self->y + 15))
                self->dir ^= 1;
            else
                self->x += 1;
        }

        // update the walking animation
        if (self->delay++ == FRAME_WAIT)
        {
            self->delay = 0;
            if (++self->frame == WALK_CYCLE)
                self->frame = 0;
        }

        // allocate the sprites
        sp.x = self->x;
        // y on the screen starts in 255
        sp.y = self->y - 1;
        // find which pattern to show
        sp.pattern = self->pat + (walk_frames[self->frame] + self->dir * 3) * 4;
        // red
        sp.attr = 9;
        spman_alloc_fixed_sprite(&sp);
    }
}

