#include "enemy_logic.h"
#include <stdint.h>
#include "game.h"
#include "game_util.h"
#include <spman.h>
#include <mplayer.h>
#include "main.h"

enum enemy_type
{
    ENEMY_STATIC = 0,
    ENEMY_MOVE,
    ENEMY_RANGE,
};

void update_enemy_static();
void update_enemy_move();
void update_enemy_range();

static uint8_t is_intersect_player()
{
    return (entities[0].roomId == self->roomId) && !gameover_delay && lives && !invuln && entities[0].x + 6 < self->x + 10 && self->x + 6 < entities[0].x + 10 && self->y == entities[0].y;
}

static void process_intersect_player()
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
}


void update_enemy()
{
    uint8_t sprite_attribute = 9;
    switch (self->extra)
    {
    case ENEMY_STATIC:
        update_enemy_static();
        sprite_attribute = 9;
        break;
    case ENEMY_RANGE:
        update_enemy_range();
        sprite_attribute = 10;
        break;
    case ENEMY_MOVE:
        update_enemy_move();
        sprite_attribute = 11;
        break;
    }

    if (g_cur_room_id == self->roomId)
    {
        sp.x = self->x;
        sp.y = self->y - 1;

        sp.pattern = self->pat + (walk_frames[self->frame] + self->dir * 3) * 4;
        sp.attr = sprite_attribute;
        spman_alloc_fixed_sprite(&sp);
    }
}



void update_enemy_static()
{
    process_intersect_player();
}

void update_enemy_range()
{
    process_intersect_player();

    int distance = (entities[0].x + g_cur_room_id  * 256) - ( self->x + self->roomId * 256);
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

    if (distance < 100 && self->y == entities[0].y && !gameover_delay)
    {
        self->dir = dir;
        if (self->dir)
        {

            if (self->x == 2)
            {
                self->x = (uint8_t)(255 - 16);
                self->roomId--;
            }
            else if (!is_map_blocked(self->x, self->y + 15))
            {
                self->x -= 1;
                
            }
        }
        else
        {
            if (self->x == 255 - 16)
            {
                self->x = 2;
                self->roomId++;
            }
            else if (!is_map_blocked(self->x + 15, self->y + 15))
            {
                self->x += 1;
               
            }
        }

        moved = 1;
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
}

void update_enemy_move()
{
    process_intersect_player();
    
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
}