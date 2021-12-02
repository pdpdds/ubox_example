#include "exit_logic.h"
#include <stdint.h>
#include "game.h"
#include "game_util.h"
#include <spman.h>

void update_exit()
{
    if (jewels != 0)
        return;

    if (self->roomId != g_cur_room_id)
        return;
        
    static uint8_t index = 0;
    if (self->delay++ == FRAME_WAIT)
    {
        self->delay = 0;
        index++;
        if (index > 1)
            index = 0;

        if (index == 1)
        {
            ubox_put_tile((self->x >> 3), (self->y >> 3), WARP_TILE + 2);
            ubox_put_tile((self->x >> 3) + 1, (self->y >> 3), WARP_TILE + 3);
            ubox_put_tile((self->x >> 3), (self->y >> 3) + 1, WARP_TILE);
            ubox_put_tile((self->x >> 3) + 1, (self->y >> 3) + 1, WARP_TILE + 1);
        }

        else
        {
            ubox_put_tile((self->x >> 3), (self->y >> 3), EXIT_TILE + 2);
            ubox_put_tile((self->x >> 3) + 1, (self->y >> 3), EXIT_TILE + 3);
            ubox_put_tile((self->x >> 3), (self->y >> 3) + 1, EXIT_TILE);
            ubox_put_tile((self->x >> 3) + 1, (self->y >> 3) + 1, EXIT_TILE + 1);
        }
    }
}