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

// generated
#include "map.h"
#include "player.h"
#include "enemy.h"

void init_map_entities()
{
    const uint8_t *m = cur_map;
    uint8_t typ, last = 0;
    uint16_t i;

    // init sprite and patterns
    spman_init();

    // this sets everything to 0, which is useful as
    // entity ET_UNUSED is 0
    memset(entities, 0, sizeof(struct entity) * MAX_ENTITIES);

    // get to the beginning of the entities:
    // map size + 3 bytes of header (the map size and the entities size)
    m += (uint16_t)(m[0] | m[1] << 8) + 3;

    // the entity list ends with 255
    while (*m != 0xff)
    {
        // first byte is type + direction flag on MSB
        // remove MSB
        typ = m[0] & (~DIR_FLAG);

        entities[last].type = typ;
        entities[last].x = m[1];
        entities[last].y = m[2];
        // in the map: param is 1 (int) to look left
        entities[last].dir = m[0] & DIR_FLAG ? DIR_LEFT : DIR_RIGHT;

        switch (typ)
        {
            // can be only one; always first entity
            // because our entities are sorted by type!
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

        // next entity
        last++;

        // all our entities are 3 bytes
        m += 3;
    }

    // count how many batteries are in the map
    batteries = 0;
    for (i = 0; i < MAP_W * MAP_H; ++i)
        if (cur_map_data[i] == BATTERY_TILE)
            batteries++;

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
void erase_battery(uint8_t x, uint8_t y)
{
    uint8_t t;
    int8_t mod;

    // find out the bg tile to use

    // border of the map
    if ((x >> 3) == 0)
        mod = 1;
    else
        mod = -1;

    switch (cur_map_data[mod + (x >> 3) + (y >> 3) * MAP_W])
    {
        case 12:
            t = 13;
            break;
        case 13:
            t = 12;
            break;
        default:
            // this is next to a wall
            if (mod == 1)
                t = 13;
            else
                t = 12;
            break;
    }

    // change the map data so we don't pick it up again
    cur_map_data[(x >> 3) + (y >> 3) * MAP_W] = t;

    // erase on the screen
    ubox_put_tile(x >> 3, y >> 3, t);
    ubox_put_tile(x >> 3, (y >> 3) - 1, t);
}

// x and y in pixels
uint8_t is_map_blocked(uint8_t x, uint8_t y)
{
    return cur_map_data[(x >> 3) + (y >> 3) * MAP_W] < LAST_SOLID_TILE + 1;
}

// x and y in pixels
uint8_t is_map_battery(uint8_t x, uint8_t y)
{
    return cur_map_data[(x >> 3) + (y >> 3) * MAP_W] == BATTERY_TILE;
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

void update_enemy()
{
    // check for the player; if alive and not invulnerable!
    // we use small hit boxes
    if (lives && !invuln
            && entities[0].x + 6 < self->x + 10 && self->x + 6 < entities[0].x + 10
            && self->y == entities[0].y)
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
}

void update_player()
{
    // to know if we need to update the walk animation
    uint8_t moved = 0;

    // player is dead
    if (!lives)
        return;

    // decrease counter if set
    if (invuln)
        invuln--;

    if (control & UBOX_MSX_CTL_RIGHT)
    {
        self->dir = DIR_RIGHT;
        moved = 1;

        // wrap horizontally
        if (self->x == 255 - 16)
            self->x = 0;
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
            self->x = (uint8_t)(255 - 16);
        // check if not solid, using bottom left
        else if (!is_map_blocked(self->x, self->y + 15))
            self->x -= 2;
    }

    // are we touching a battery?
    // use bottom center
    if (is_map_battery(self->x + 8, self->y + 15))
    {
        mplayer_play_effect_p(EFX_BATTERY, EFX_CHAN_NO, 0);
        batteries--;
        erase_battery(self->x + 8, self->y + 15);
    }

    if (control & UBOX_MSX_CTL_FIRE1)
    {
        // use flags to prevent repeat: the player will
        // have to release fire to use the elevator again
        if (!self->flags)
        {
            self->flags = 1;

            // check elevator down; using bottom middle
            if (is_map_elevator_down(self->x + 8, self->y + 16))
            {
                mplayer_play_effect_p(EFX_ELEVATOR, EFX_CHAN_NO, 0);
                self->y += 8 * 4;
            }
            // then elevator up; using bottom middle
            else if (is_map_elevator_up(self->x + 8, self->y + 16))
            {
                mplayer_play_effect_p(EFX_ELEVATOR, EFX_CHAN_NO, 0);
                self->y -= 8 * 4;
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

void run_game()
{
    uint8_t i;

    // init some variables; look at game.h for description
    lives = MAX_LIVES;
    invuln = 0;
    gameover_delay = 0;

    ubox_disable_screen();

    ubox_fill_screen(WHITESPACE_TILE);

    // we only have one map, select it
    cur_map = map[0];
    // uncompress map data into RAM, we will modify it
    // map data starts on byte 3 (skip map data size and entities size)
    ap_uncompress(cur_map_data, cur_map + 3);

    // init entities before drawing
    init_map_entities();
    draw_map();

    draw_hud();

    ubox_enable_screen();

    mplayer_init(SONG, SONG_IN_GAME);

    // our game loop
    while (1)
    {
        // exit the game
        if (ubox_read_keys(7) == UBOX_MSX_KEY_ESC)
            break;

        // game completed!
        if (!batteries)
            break;

        // we are in the gameover delay
        if (gameover_delay)
        {
            // if finished, exit
            if (--gameover_delay == 0)
                break;
        }

        // read the selected control
        control = ubox_read_ctl(ctl);

        // update all the entities:
        // - self is a pointer to THIS entity
        // - because we don't create/destroy entities dynamically
        //   when we found one that is unused we are done
        for (i = 0, self = entities; i < MAX_ENTITIES && self->type; i++, self++)
            self->update();

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
