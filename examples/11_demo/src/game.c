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

#include "map_summary.h"

#include "player.h"
#include "enemy.h"
#include "foothold.h"

#include "foothold_logic.h"
#include "enemy_logic.h"
#include "player_logic.h"


const uint8_t walk_frames[WALK_CYCLE] = {0, 1, 0, 2};
uint8_t g_maxEntities = 0;



static uint8_t get_entity_count(const uint8_t *mapData)
{
    uint8_t entityCount = 0;

    const uint8_t *m = (const uint8_t *)(mapData);

    //map header is 3 bytes. map size 2byte + 0x00 1 byte
    m += (uint16_t)(m[0] | m[1] << 8) + 3;

    //(map pointer + 3 bytes + map_size) indicates entities. 
    //entity size is 5 bytes. id, type, x, y, type. extra
    //if *m is 0xff it means map data over.
    while (*m != 0xff)
    {
        entityCount++;
        
        m += 5;
    }

    return entityCount;
}

static void init_map_entities(uint8_t stage)
{
    uint8_t i = 0;
    uint8_t mapCount = 1;
    spman_init();

    cur_map = g_map[stage - 1];
    
    mapCount = g_map_room_count[stage - 1];
    
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
            case ET_FOOTHOLD:
                // 3 frames
                entities[last].pat = spman_alloc_pat(PAT_FOOTHOLD, foothold[0], 3, 0);
                entities[last].update = update_foothold;
                break;
            }

            last++;

            // entity is 5 bytes
            m += 5;
        }
    }
}

static void draw_static_object()
{
    uint8_t i = 0;
    struct entity *object;
    for (i = 0, object = entities; i < g_maxEntities; i++, object++)
    {
        if (g_cur_map_id != object->mapid || !object->type)
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

static void draw_map()
{
    ubox_wait_vsync();
    ubox_write_vm((uint8_t *)0x1800, MAP_W * MAP_H, cur_map_data);
}

static void draw_hud()
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

struct entity* check_foothold(uint8_t x, uint8_t y)
{
    uint16_t i;
    struct entity *object;

    for (i = 0, object = entities; i < g_maxEntities; i++, object++)
    {
        if (object->type == ET_FOOTHOLD && g_cur_map_id == object->mapid)
        {
            if ((x >= object->x && (x <= (object->x + 16))) && y == object->y)
            {
                return object;
            }
        }
    }

    return 0;
}

uint8_t check_floor(uint8_t x, uint8_t y)
{
    uint8_t t = cur_map_data[(x >> 3) + (y >> 3) * MAP_W];
    if (t <= LAST_SOLID_TILE)
        return 1;

    return 0;
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
    g_player_info.state = PS_NORMAL;

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

        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
            break;

        if (g_gamestate == STATE_GAME_CLEAR)
            break;

        if (gameover_delay)
        {
            if (--gameover_delay == 0)
            {
                break;
            }
        }

        control = ubox_read_ctl(ctl);

        for (i = 0, self = entities; i < g_maxEntities; i++, self++)
        {

            if (self->type && self->mapid == g_cur_map_id)
                self->update();
        }

        ubox_wait();
        spman_update();
    }

    mplayer_init(SONG, SONG_SILENCE);
    spman_hide_all_sprites();
}
