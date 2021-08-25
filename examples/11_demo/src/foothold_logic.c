#include <stdint.h>
#include "game.h"
#include <spman.h>

uint8_t update_player_foothold()
{
    uint8_t moved = update_player_move();

    struct entity *object = find_collide_object(self->x + 8, self->y + 16, ET_FOOTHOLD);

    if (object)
    {
        if (object->delay == 0)
        {

            if ((!object->dir) && !is_map_blocked(self->x + 12, self->y + 15))
            {
                self->x += 1;
            }
            else if (object->dir && !is_map_blocked(self->x + 4, self->y + 15))
                self->x -= 1;
        }
    }

    return moved;
}


void update_foothold()
{
    if (self->delay > 0)
    {
        self->delay--;
    }
    else
    {
        if (self->dir)
        {
            if (is_map_blocked(self->x, self->y + 7))
            {
                self->delay = FOOTHOLD_DELAY;
                self->dir ^= 1;
            }
            else
                self->x -= 1;
        }
        else
        {
            if (is_map_blocked(self->x + 15, self->y + 7))
            {
                self->delay = FOOTHOLD_DELAY;
                self->dir ^= 1;
            }
            else
                self->x += 1;
        }
    }

    if (g_cur_map_id == self->mapid)
    {
        sp.x = self->x;
        sp.y = self->y - 1;

        sp.pattern = self->pat;
        sp.attr = 10;
        spman_alloc_fixed_sprite(&sp);
    }
}