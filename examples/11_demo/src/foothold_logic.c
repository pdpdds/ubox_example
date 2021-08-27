#include <stdint.h>
#include "game.h"
#include "game_util.h"
#include <spman.h>


#define FOOTHOLD_DELAY 36

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

    if (g_cur_room_id == self->roomId)
    {
        sp.x = self->x;
        sp.y = self->y - 1;

        sp.pattern = self->pat;
        sp.attr = 10;
        spman_alloc_fixed_sprite(&sp);
    }
}