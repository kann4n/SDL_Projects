#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>

#define SCRN_WIDTH  800
#define SCRN_HEIGHT 600
#define BALL_SPEED  200.0f
#define SPEED       400.0f
#define WIN_SCORE   10
#define PLYR_WIDTH  10
#define PLYR_HEIGHT 50
#define PLYR_OFFSET 10
#define BALL_ANGVY  0.75    // must be [0, 1]

typedef enum { Ready, Lost, Won } PlayerStatus;

typedef struct
{
    SDL_FRect     rect;
    SDL_Texture  *texture;
    PlayerStatus  status;
    int           score;
} Player;

typedef struct
{
    float               r;
    SDL_FRect        rect;
    SDL_Texture  *texture;
    float          vx, vy;
} Ball;

typedef struct
{
    Player left;
    Player right;
    Ball   ball;
    bool   running;
} Game;

typedef struct
{
    bool move_up;
    bool move_down;
} PlayerControls;

extern const SDL_Color fg;
extern const SDL_Color bg;

void reset_ball(Game *game);
bool circle_rect_intersection(Ball b, SDL_FRect rect);
void update_player(Player *p, bool up, bool down, float dt);
void update_ball(Game *game, float dt);
void game_over_screen(SDL_Renderer *renderer, char *over_msg, TTF_Font *font);
void display_score(SDL_Renderer *renderer, TTF_Font *font, int score1, int score2);


#endif