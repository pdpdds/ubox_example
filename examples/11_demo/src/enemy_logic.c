#include "enemy_logic.h"
#include <stdint.h>
#include "game.h"
#include <spman.h>
#include <mplayer.h>
#include "main.h"


static uint8_t is_intersect_player()
{
    return !gameover_delay && lives && !invuln && entities[0].x + 6 < self->x + 10 && self->x + 6 < entities[0].x + 10 && self->y == entities[0].y;
}

void update_enemy()
{
    if (self->extra == ENEMY_STATIC)
    {
        if (is_intersect_player())
        {
            lives--;

            invuln = INVUL_TIME;

            if (!lives)
            {
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
    else if (self->extra == ENEMY_RANGE)
    {
        if (is_intersect_player())
        {
            self->dir ^= 1;
            lives--;

            invuln = INVUL_TIME;

            if (!lives)
            {
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

        int distance = entities[0].x - self->x;
        uint8_t dir = 0;
        uint8_t moved = 0;
        if (distance < 0)
        {
            dir = 1;
            distance = -distance;
        }
        else
        {
            dir = 0;
        }

        if (distance < 80 && self->y == entities[0].y && !gameover_delay)
        {
            self->dir = dir;
            if (self->dir)
            {
                if (self->x == 2 || is_map_blocked(self->x, self->y + 15))
                    self->dir ^= 1;
                else
                {
                    self->x -= 1;
                    moved = 1;
                }
            }
            else
            {
                if (self->x == 255 - 16 || is_map_blocked(self->x + 15, self->y + 15))
                    self->dir ^= 1;
                else
                {
                    self->x += 1;
                    moved = 1;
                }
            }
        }

        if (moved)
        {
            if (self->delay++ == FRAME_WAIT)
            {
                self->delay = 0;
                if (++self->frame == WALK_CYCLE)
                    self->frame = 0;
            }
        }
        else
        {
            self->delay = 0;
            self->frame = 0;
        }

        sp.x = self->x;
        sp.y = self->y - 1;
        sp.pattern = self->pat + (walk_frames[self->frame] + self->dir * 3) * 4;
        sp.attr = 11;
        spman_alloc_fixed_sprite(&sp);
    }
    else if (self->extra == ENEMY_MOVE)
    {
        if (is_intersect_player())
        {
            self->dir ^= 1;

            lives--;

            invuln = INVUL_TIME;

            if (!lives)
            {
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


        if (self->dir)
        {
            if (self->x == 2 || is_map_blocked(self->x, self->y + 15))
                self->dir ^= 1;
            else
                self->x -= 1;
        }
        else
        {
            if (self->x == 255 - 16 || is_map_blocked(self->x + 15, self->y + 15))
                self->dir ^= 1;
            else
                self->x += 1;
        }

        if (self->delay++ == FRAME_WAIT)
        {
            self->delay = 0;
            if (++self->frame == WALK_CYCLE)
                self->frame = 0;
        }

        sp.x = self->x;
        sp.y = self->y - 1;

        sp.pattern = self->pat + (walk_frames[self->frame] + self->dir * 3) * 4;
        sp.attr = 9;
        spman_alloc_fixed_sprite(&sp);
    }
}
