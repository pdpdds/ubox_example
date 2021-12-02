#ifndef _GAME_H
#define _GAME_H

#ifndef LOCAL
#define LOCAL extern
#else

#define WALK_CYCLE 4

// walk animation frames, private
const uint8_t walk_frames[WALK_CYCLE] = { 0, 1, 0, 2 };

#endif

/**
 * Game implementation and helpers.
 */

// map size in tiles
#define MAP_W 32
#define MAP_H 21

// some useful tiles
#define LAST_SOLID_TILE     10
#define BATTERY_TILE        224

#define MAX_LIVES 3

// player + other entities
#define MAX_ENTITIES 11

// MSB in our entity data
#define DIR_FLAG 128

// sprite direction
#define DIR_LEFT 1
#define DIR_RIGHT 0

// how long we wait between animation frames
#define FRAME_WAIT 3

// how long the player is invulnerable after death
// time in frames
#define INVUL_TIME 64

// show some game frames before jumping to the game over
// screen; this helps the player to know what happened
#define GAMEOVER_DELAY 72

// types for our pattern groups
// used by spman
enum pattern_type
{
    PAT_PLAYER = 0,
    PAT_PLAYER_FLIP,
    PAT_ENEMY,
    PAT_ENEMY_FLIP,
};

// entity types in the same order
// used in the map (see map_conf.json)
enum entity_type
{
    ET_UNUSED = 0,
    ET_PLAYER,
    ET_ENEMY,
};

// notes:
//
//  - x, y are the logic position on the map
//  - sp.x, sp.y are the position on screen
struct entity
{
    uint8_t type;
    uint8_t x;
    uint8_t y;
    uint8_t dir;
    uint8_t pat;
    uint8_t flags;
    uint8_t delay;
    uint8_t frame;
    void (*update)();
};

void run_game();

void update_player();
void update_enemy();

void draw_map();
void draw_hud();

void erase_battery(uint8_t x, uint8_t y);

uint8_t is_map_blocked(uint8_t x, uint8_t y);
uint8_t is_map_elevator_up(uint8_t x, uint8_t y);
uint8_t is_map_elevator_down(uint8_t x, uint8_t y);

// our entities; the player is 0
LOCAL struct entity entities[MAX_ENTITIES];

// used to read our control method
LOCAL uint8_t control;

// current map; pointer in ROM
LOCAL const uint8_t *cur_map;
// current map tile map (map data, not entities)
// we don't use ROM because we will modify it
LOCAL uint8_t cur_map_data[MAP_W * MAP_H];

// player lives
LOCAL uint8_t lives;
// invulnerability time after death
LOCAL uint8_t invuln;
// batteries left
LOCAL uint8_t batteries;
// show some delay once the lives run out
LOCAL uint8_t gameover_delay;

// used by all entities
LOCAL struct sprite_attr sp;

// current entity
LOCAL struct entity *self;

#ifdef LOCAL
#undef LOCAL
#endif

#endif // _GAME_H
