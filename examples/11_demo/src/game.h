#ifndef _GAME_H
#define _GAME_H

#ifndef LOCAL
#define LOCAL extern
#else

#define WALK_CYCLE 4

const uint8_t walk_frames[WALK_CYCLE] = { 0, 1, 0, 2 };

#endif

#define MAP_W 32
#define MAP_H 21

#define LAST_SOLID_TILE     10
#define BATTERY_TILE        224
#define JEWEL_TILE          194
#define WARP_TILE           15
#define EXIT_TILE           19
#define BLANK_TILE          0

#define MAX_LIVES 3
#define MAX_LEVEL 5

#define MAX_ENTITIES 50

#define DIR_FLAG 128

#define DIR_LEFT 1
#define DIR_RIGHT 0
#define FRAME_WAIT 3
#define INVUL_TIME 64
#define GAMEOVER_DELAY 72

enum pattern_type
{
    PAT_PLAYER = 0,
    PAT_PLAYER_FLIP,
    PAT_ENEMY,
    PAT_ENEMY_FLIP,
};

enum GAME_STATE
{
    STATE_TITLE = 0,
    STATE_IN_GAME,
    STATE_NO_MAP,
    STATE_GAME_OVER,
    STATE_GAME_CLEAR,
    STATE_GAME_RESET,
};



enum entity_type
{
    ET_UNUSED = 0,
    ET_PLAYER,
    ET_ENEMY,
    ET_EXIT,
    ET_KEY,
    ET_WARP,
};

enum enemy_type
{
    ENEMY_STATIC = 0,
    ENEMY_MOVE,
};

struct entity
{
    uint8_t type;
    uint8_t identifier;
    uint8_t mapid;
    uint8_t x;
    uint8_t y;
    uint8_t extra;
    uint8_t dir;
    uint8_t pat;
    uint8_t flags;
    uint8_t delay;
    uint8_t frame;
    void (*update)();
};

void run_game(int stage);

void update_player();
void update_enemy();
void update_item();
void update_warp();
void update_exit();

void draw_map();
void draw_hud();

void erase_battery(uint8_t x, uint8_t y);

uint8_t is_map_blocked(uint8_t x, uint8_t y);
uint8_t is_map_elevator_up(uint8_t x, uint8_t y);
uint8_t is_map_elevator_down(uint8_t x, uint8_t y);

LOCAL struct entity entities[MAX_ENTITIES];

LOCAL uint8_t control;

LOCAL const uint8_t** cur_map;
LOCAL uint8_t cur_map_data[MAP_W * MAP_H];

LOCAL uint8_t lives;
LOCAL uint8_t invuln;
LOCAL uint8_t jewels;
LOCAL uint8_t gameover_delay;

LOCAL struct sprite_attr sp;
LOCAL struct entity *self;

#ifdef LOCAL
#undef LOCAL
#endif

#endif // _GAME_H
