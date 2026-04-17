#ifndef KENGINE_H
#define KENGINE_H

#include <SDL3/SDL.h>

#define SCREEN_WID 800
#define SCREEN_HIG 800
#define MAP_XB 32
#define MAP_YB 32
#define TILE_SIZE 50
#define MAP_SIZE 100
#define MAPTILE_WIDTH (MAP_SIZE / MAP_XB)
#define MAPTILE_HIGHT (MAP_SIZE / MAP_YB)
#define WALLWIDTH 1
#define RAY_KOUNT (SCREEN_WID / WALLWIDTH)

// helpers
#define M_PI 3.14159265358979323846
#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0f)


extern int map[MAP_YB][MAP_XB];


struct Player
{
    float x, y;
    float size;
    float speed;
    float angle;
    float rotSpeed; // in rad
    float fov;      // in rad
};

struct PlayerControls
{
    int moveForward, moveBack;
    int moveLeft, moveRight;
    int rotLeft, rotRight;
};

void draw_map(SDL_Renderer *renderer);
void draw_player(SDL_Renderer *renderer, struct Player player);
void draw_3d(SDL_Renderer *renderer, struct Player player);
void update_player(struct Player *player, struct PlayerControls playerCtrl);

#endif