#include <stdint.h>
#include <string.h>

#include "ubox.h"
#include "spman.h"
#include "mplayer.h"
#include "ap.h"

#include "helpers.h"
#include "main.h"

#define LOCAL
#include "game.h"

#include "map.h"
#include "map2.h"
#include "map3.h"
#include "map4.h"
#include "player.h"
#include "enemy.h"

extern uint8_t g_gamestate;

struct entity *find_collide_object(uint8_t x, uint8_t y, int type);

uint8_t get_entity_count(const uint8_t *mapData)
{
    uint8_t entityCount = 0;

    const uint8_t *m = (const uint8_t *)(mapData);

    m += (uint16_t)(m[0] | m[1] << 8) + 3;

    while (*m != 0xff)
    {
        entityCount++;

        // 엔터티는 5바이트로 표현됨
        m += 5;
    }

    return entityCount;
}

uint8_t g_maxEntities = 0;
uint8_t g_cur_map_id = 0;
void init_map_entities(uint8_t stage)
{
    uint8_t i = 0;    
    uint8_t mapCount = 1;
    spman_init();

    if (stage == 1)
        cur_map = map;
    else if (stage == 2)
        cur_map = map2;  
    else if (stage == 3)
        cur_map = map3;
    else if (stage == 4)
    {
        mapCount = 2;
        cur_map = map4;
    }
    else
        cur_map = map;
        
        


    for (i = 0; i < mapCount; i++)
    {
        g_maxEntities += get_entity_count(cur_map[i]);
    }

    uint8_t typ, last = 0;
    
    memset(entities, 0, sizeof(struct entity) * MAX_ENTITIES);

    for (i = 0; i < mapCount; i++)
    {
        const uint8_t *m = (const uint8_t *)(cur_map[i]);

        m += (uint16_t)(m[0] | m[1] << 8) + 3;

        while (*m != 0xff)
        {

            typ = m[0] & (~DIR_FLAG);

            entities[last].type = typ;
            entities[last].mapid = i;
            entities[last].x = m[1];
            entities[last].y = m[2];
            entities[last].identifier = m[3];
            entities[last].extra = m[4];
            
            entities[last].dir = m[0] & DIR_FLAG ? DIR_LEFT : DIR_RIGHT;

            switch (typ)
            {
            case ET_KEY:
                entities[last].update = update_item;
                jewels++;
                break;

            case ET_WARP:
                entities[last].update = update_warp;
                break;

            case ET_EXIT:
                entities[last].update = update_exit;
                break;

            case ET_PLAYER:
                // 3 frames x 2 sprites = 6
                entities[last].pat = spman_alloc_pat(PAT_PLAYER, player_sprite[0], 6, 0);
                spman_alloc_pat(PAT_PLAYER_FLIP, player_sprite[0], 6, 1);
                entities[last].update = update_player;
                break;

            case ET_ENEMY:
                // 3 frames
                entities[last].pat = spman_alloc_pat(PAT_ENEMY, enemy_sprite[0], 3, 0);
                spman_alloc_pat(PAT_ENEMY_FLIP, enemy_sprite[0], 3, 1);
                entities[last].update = update_enemy;
                break;
            }

            last++;

            // 엔터티는 5바이트로 표현됨
            m += 5;
        }
    }
}

void draw_static_object()
{
    uint8_t i = 0;
    struct entity *object;
    for (i = 0, object = entities; i < g_maxEntities; i++, object++)
    {
        if(g_cur_map_id != object->mapid || !object->type)
            continue;

        if (object->type == ET_KEY)
            ubox_put_tile(object->x >> 3, object->y >> 3, JEWEL_TILE);

        if (object->type == ET_WARP)
        {
            ubox_put_tile((object->x >> 3), (object->y >> 3), WARP_TILE + 2);
            ubox_put_tile((object->x >> 3) + 1, (object->y >> 3), WARP_TILE + 3);
            ubox_put_tile((object->x >> 3), (object->y >> 3) + 1, WARP_TILE);
            ubox_put_tile((object->x >> 3) + 1, (object->y >> 3) + 1, WARP_TILE + 1);
        }
    }
}

void draw_map()
{
    // draw the map!
    //
    //  - is not compressed (which limits how many maps we can store)
    //  - our map is just the tile numbers
    //
    // so we do it in one call by coping our map to the backtround tile map
    // addr in the VDP VRAM
    ubox_wait_vsync();
    ubox_write_vm((uint8_t *)0x1800, MAP_W * MAP_H, cur_map_data);
}

void draw_hud()
{
    uint8_t i;

    put_text(0, 21, "LIVES");

    for (i = 0; i < MAX_LIVES; ++i)
        if (i < lives)
            // our hearts tile
            ubox_put_tile(1 + i, 22, 193);
        else
            ubox_put_tile(1 + i, 22, WHITESPACE_TILE);
}

// x and y in pixels
uint8_t is_map_blocked(uint8_t x, uint8_t y)
{
    return cur_map_data[(x >> 3) + (y >> 3) * MAP_W] < LAST_SOLID_TILE + 1;
}

uint8_t is_map_jewel(uint8_t x, uint8_t y)
{
    uint16_t i;
    struct entity *object;
    for (i = 0, object = entities; i < g_maxEntities; i++, object++)
    {
        
        if (object->type == ET_KEY)
        {
            if (((x >> 3) == (object->x >> 3)) && ((y >> 3) == (object->y >> 3)))
            {
                ubox_put_tile(object->x >> 3, object->y >> 3, BLANK_TILE);
                object->type = ET_UNUSED;
                jewels--;
                return 1;
            }
        }
    }

    return 0;
}

// x and y in pixels; always check the bottom tile!
uint8_t is_map_elevator_down(uint8_t x, uint8_t y)
{
    uint8_t t = cur_map_data[(x >> 3) + (y >> 3) * MAP_W];

    // first check the elevator platform comparing tiles
    if (t != 9 && t != 10)
        return 0;

    // then check the elevator tube comparing tiles
    t = cur_map_data[(x >> 3) + ((y >> 3) - 1) * MAP_W];
    return (t != 14 && t != 15);
}

// x and y in pixels; always check the bottom tile!
uint8_t is_map_elevator_up(uint8_t x, uint8_t y)
{
    uint8_t t = cur_map_data[(x >> 3) + (y >> 3) * MAP_W];

    // first check the elevator platform comparing tiles
    if (t != 9 && t != 10)
        return 0;

    // then check the elevator tube comparing tiles
    t = cur_map_data[(x >> 3) + ((y >> 3) - 1) * MAP_W];
    return (t == 14 || t == 15);
}

struct entity *find_object(uint8_t id)
{
    uint16_t i;
    struct entity *object;
    for (i = 0, object = entities; i < g_maxEntities; i++, object++)
    {
        if (id == object->identifier)
            return object;
    }

    return 0;
}

struct entity *find_collide_object(uint8_t x, uint8_t y, int type)
{
    uint16_t i;
    struct entity *object;

    for (i = 0, object = entities; i < g_maxEntities; i++, object++)
    {
        if (object->type == type && g_cur_map_id == object->mapid)
        {
            if (x > object->x - 4 && (x + 4) < (object->x + 16) && y == object->y)
            {
                return object;
            }
        }
    }

    return 0;
}

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

    /*else if (self->extra == ENEMY_MOVE)
    {
        if (lives && !invuln && entities[0].x + 6 < self->x + 10 && self->x + 6 < entities[0].x + 10 && self->y == entities[0].y)
        {
            // change direction
            self->dir ^= 1;

            // remove one life (is more like "hits")
            lives--;
            draw_hud();
            invuln = INVUL_TIME;

            if (!lives)
            {
                // different sound effects if is game over
                mplayer_init(SONG, SONG_SILENCE);
                mplayer_play_effect_p(EFX_DEAD, EFX_CHAN_NO, 0);
                gameover_delay = GAMEOVER_DELAY;
            }
            else
                mplayer_play_effect_p(EFX_HIT, EFX_CHAN_NO, 0);
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
        spman_alloc_sprite(&sp);
    }*/
}

void move_next_map(uint8_t mapId)
{

    ubox_disable_screen();
    ubox_fill_screen(WHITESPACE_TILE);    

    ap_uncompress(cur_map_data, cur_map[mapId] + 3);
    
    draw_map();
    draw_static_object();
    
    draw_hud();

    ubox_enable_screen();
}

void update_item()
{
}

void update_warp()
{
}

void update_exit()
{
    if (jewels != 0)
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

void update_player()
{
    // to know if we need to update the walk animation
    uint8_t moved = 0;

    // player is dead
    //if (!lives)
       // return;

    // decrease counter if set
    if (invuln)
        invuln--;

    if(!gameover_delay)
    {
        if (control & UBOX_MSX_CTL_RIGHT)
    {
        self->dir = DIR_RIGHT;
        moved = 1;

        // wrap horizontally
        if (self->x == 256 - 16)
        {
            g_cur_map_id += 1;
            self->x = 16;
            self->mapid = g_cur_map_id;

            move_next_map(g_cur_map_id);
            
        }
        // check if not solid, using bottom right
        else if (!is_map_blocked(self->x + 15, self->y + 15))
            self->x += 2;
    }

    if (control & UBOX_MSX_CTL_LEFT)
    {
        self->dir = DIR_LEFT;
        moved = 1;

        // wrap horizontally
        if (self->x == 2)
        {
            g_cur_map_id -= 1;
            self->mapid = g_cur_map_id;
            self->x = (uint8_t)(256 - 16);

            move_next_map(g_cur_map_id);
            
            
        }
        // check if not solid, using bottom left
        else if (!is_map_blocked(self->x, self->y + 15))
            self->x -= 2;
    }

    is_map_jewel(self->x + 8, self->y + 15);

    if (control & UBOX_MSX_CTL_FIRE1)
    {
        // use flags to prevent repeat: the player will
        // have to release fire to use the elevator again
        if (!self->flags)
        {
            self->flags = 1;

            struct entity *object = find_collide_object(self->x, self->y, ET_WARP);

            if (object)
            {
                mplayer_play_effect_p(EFX_ELEVATOR, EFX_CHAN_NO, 0);
                struct entity *next = find_object(object->extra);

                if (next->mapid != self->mapid)
                {
                    g_cur_map_id = next->mapid;                 
                    self->mapid = g_cur_map_id;

                    move_next_map(next->mapid);                      
                }

                self->x = next->x;
                self->y = next->y;
            }           
        }
    }
    else
    {
        if (self->flags)
            self->flags = 0;
    }

    // it moved, or at least pushed against a wall
    if (moved)
    {
        // update the walking animation
        if (self->delay++ == FRAME_WAIT)
        {
            self->delay = 0;
            if (++self->frame == WALK_CYCLE)
                self->frame = 0;
        }
    }
    else
    {
        // just stand
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

    // if we are invulnerable, don't draw odd frames
    // and we get a nice blinking effect
    if (invuln & 1)
        return;

    // allocate the player sprites; fixed so they never flicker
    sp.x = self->x;
    // y on the screen starts in 255
    sp.y = self->y - 1;
    // find which pattern to show
    sp.pattern = self->pat + (walk_frames[self->frame] + self->dir * 3) * 8;
    // green
    sp.attr = 12;
    spman_alloc_fixed_sprite(&sp);
    // second one is 4 patterns away (16x16 sprites)
    sp.pattern += 4;
    // white
    sp.attr = 15;
    spman_alloc_fixed_sprite(&sp);
}

void run_game(int stage)
{
    uint8_t i;
    invuln = 0;
    gameover_delay = 0;
    g_cur_map_id = 0;
    g_maxEntities = 0;
    jewels = 0;

    g_gamestate = STATE_IN_GAME;    

    ubox_disable_screen();
    ubox_fill_screen(WHITESPACE_TILE);

    init_map_entities(stage);
  
    ap_uncompress(cur_map_data, cur_map[g_cur_map_id] + 3);

    draw_map();

    draw_static_object();

    draw_hud();

    ubox_enable_screen();

    mplayer_init(SONG, SONG_IN_GAME);

    while (1)
    {

        // exit the game
        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
            break;

        if (g_gamestate == STATE_GAME_CLEAR)
            break;

        // we are in the gameover delay
        if (gameover_delay)
        {
            // if finished, exit
            if (--gameover_delay == 0)
            {
                break;
            }
        }

        // read the selected control
        control = ubox_read_ctl(ctl);

        // update all the entities:
        // - self is a pointer to THIS entity
        // - because we don't create/destroy entities dynamically
        //   when we found one that is unused we are done
        for (i = 0, self = entities; i < g_maxEntities; i++, self++)
        {
            
            if (self->type && self->mapid == g_cur_map_id)
                self->update();            
        }

        // ensure we wait to our desired update rate
        ubox_wait();
        // update sprites on screen
        spman_update();
    }

    // stop the in game music
    mplayer_init(SONG, SONG_SILENCE);
    // hide all the sprites before going back to the menu
    spman_hide_all_sprites();
}
