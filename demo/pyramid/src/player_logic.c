#include "player_logic.h"
#include "game.h"
#include "game_util.h"
#include <spman.h>
#include <mplayer.h>
#include "main.h"
#include "foothold_logic.h"

uint8_t update_player_foothold();

uint8_t check_player_floor()
{
    return check_floor(self->x + 4, self->y + 16) || check_floor(self->x + 12, self->y + 16);
}

struct entity* check_player_foothold()
{
    struct entity *object = check_foothold(self->x + 4, self->y + 16);

    if (object == 0)
    {
        object = check_foothold(self->x + 12, self->y + 16);
    }

    return object;
}

uint8_t update_player_move()
{
    uint8_t moved = 0;
    if (control & UBOX_MSX_CTL_RIGHT)
    {
        self->dir = DIR_RIGHT;
        moved = 1;

        if (self->x == 256 - 16)
        {
            g_cur_room_id += 1;
            self->x = 2;
            self->roomId = g_cur_room_id;

            move_next_room(g_cur_room_id);
        }
        else if (!is_map_blocked(self->x + 15, self->y + 15))
            self->x += 2;
    }

    if (control & UBOX_MSX_CTL_LEFT)
    {
        self->dir = DIR_LEFT;
        moved = 1;

        if (self->x == 2)
        {
            g_cur_room_id -= 1;
            self->roomId = g_cur_room_id;
            self->x = (uint8_t)(256 - 16);

            move_next_room(g_cur_room_id);
        }
        else if (!is_map_blocked(self->x, self->y + 15))
            self->x -= 2;
    }

    is_map_jewel(self->x + 8, self->y + 15);

    if (control & UBOX_MSX_CTL_FIRE1)
    {

        if (!self->flags)
        {
            self->flags = 1;

            struct entity *object = find_collide_object(self->x, self->y, ET_WARP);

            if (object)
            {
                mplayer_play_effect_p(EFX_ELEVATOR, EFX_CHAN_NO, 0);
                struct entity *next = find_object(object->extra);

                if (next)
                {
                    if (next->roomId != self->roomId)
                    {
                        g_cur_room_id = next->roomId;
                        self->roomId = g_cur_room_id;

                        move_next_room(next->roomId);
                    }

                    self->x = next->x;
                    self->y = next->y;
                }
            }
        }
    }
    else
    {
        if (self->flags)
            self->flags = 0;
    }

    return moved;
}

void update_player_fall()
{
    self->y += 4;

    if(check_player_floor())
    {
        g_player_info.state = PS_NORMAL;
    }
}


void update_player_state(uint8_t moved)
{
    if (g_player_info.state == PS_FALL)
    {
        if (check_player_floor())
        {
            g_player_info.state = PS_NORMAL;
        }
        else if (check_player_foothold())
        {

            g_player_info.state = PS_FOOTHOLD;
        }
    }

    else if (g_player_info.state == PS_NORMAL && moved)
    {
        uint8_t loc_x = self->x;
        uint8_t loc_y = self->y + 16;

        if(!check_player_floor())
        {
            if(check_player_foothold())
            {
                g_player_info.state = PS_FOOTHOLD;
            }
            else
            {
                g_player_info.state = PS_FALL;
            }
        }
    }
    else if (g_player_info.state == PS_FOOTHOLD && moved)
    {
        struct entity *object = check_player_foothold();
        
        if (!object)
        {
            if (check_player_floor())
            {
                g_player_info.state = PS_NORMAL;
            }
            else
                g_player_info.state = PS_FALL;
        }
    }
}

void update_player()
{
    uint8_t moved = 0;

    if (invuln)
        invuln--;

    if (!gameover_delay)
    {
        if (g_player_info.state == PS_NORMAL)
        {
            moved = update_player_move();
        }
        else if (g_player_info.state == PS_FALL)
        {
            update_player_fall();
        }
        else if (g_player_info.state == PS_FOOTHOLD)
        {
            moved = update_player_foothold();
        }

        // 플레이어가 움직였다면 애니메이션을 갱신한다.
        if (moved)
        {
            //걷기 애니메이션을 갱신
            if (self->delay++ == FRAME_WAIT)
            {
                self->delay = 0;
                if (++self->frame == WALK_CYCLE)
                    self->frame = 0;
            }
        }
        else
        {
            //움직이지 않았다면 단순히 서있는 스프라이트를 그린다.
            if (self->frame)
            {
                self->frame = 0;
                self->delay = 0;
            }
        }

        struct entity *exitobject = find_collide_object(self->x, self->y, ET_EXIT);

        if (exitobject && jewels == 0)
            g_gamestate = STATE_GAME_CLEAR;
    }

    update_player_state(moved);

    if (invuln & 1)
        return;

    sp.x = self->x;
    sp.y = self->y - 1;

    sp.pattern = self->pat + (walk_frames[self->frame] + self->dir * 3) * 8;
    sp.attr = 12;
    spman_alloc_fixed_sprite(&sp);

    sp.pattern += 4;
    sp.attr = 15;
    spman_alloc_fixed_sprite(&sp);
}

uint8_t update_player_foothold()
{
    uint8_t moved = update_player_move();

    struct entity *object = check_player_foothold();

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
