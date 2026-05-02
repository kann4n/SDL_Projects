#include "game.h"
#include <SDL3/SDL_stdinc.h>

const SDL_Color fg = { 0x00, 0x00, 0x00, 0xff };
const SDL_Color bg = { 0xfc, 0xfb, 0xfa, 0xff };

void reset_ball(Game *game)
{
    game->ball.rect.x = (SCRN_WIDTH  / 2.0f) - game->ball.r;
    game->ball.rect.y = (SCRN_HEIGHT / 2.0f) - game->ball.r;
    game->ball.vx = (SDL_rand(2) == 0 ? 1 : -1) * BALL_SPEED;
    game->ball.vy = (SDL_rand(2) == 0 ? 1 : -1) * BALL_SPEED;
}

bool circle_rect_intersection(Ball b, SDL_FRect rect)
{
    // center of ball
    float cenX = b.rect.x + b.r;
    float cenY = b.rect.y + b.r;
    // closet point on rect from ball
    float cloX = SDL_clamp(cenX, rect.x, rect.x + rect.w);
    float cloY = SDL_clamp(cenY, rect.y, rect.y + rect.h);
    // distence
    float disX = cloX - cenX;
    float disY = cloY - cenY;
    // check distence
    float disSqr = disX*disX + disY*disY;
    return disSqr < (b.r * b.r);
}

void update_player(Player *p, bool up, bool down, float dt)
{
    float dir = (float)down - (float)up;
    p->rect.y += dir * SPEED * dt;
    p->rect.y = SDL_clamp(p->rect.y, 0, SCRN_HEIGHT - p->rect.h);
}

void update_ball(Game *game, float dt)
{
    Ball *b = &game->ball;

    // move
    b->rect.x += b->vx * dt;
    b->rect.y += b->vy * dt;

    // top-bottom
    if (b->rect.y < 0)
    {
        b->vy = -b->vy;
        b->rect.y = 0;
    }
    else if (b->rect.y + b->rect.h > SCRN_HEIGHT)
    {
        b->vy = -b->vy;
        b->rect.y = SCRN_HEIGHT - b->rect.h;
    }

    // col-check
    if (circle_rect_intersection(*b, game->left.rect))
    {
        float padC  = game->left.rect.y + game->left.rect.h / 2.0f;
        float ballC = game->ball.rect.y + game->ball.r;
        float ypact = 2.0f * (ballC - padC) / game->left.rect.h;    // [-1, 1]
    
        float speed = SDL_fabsf(b->vx);
        b->vx = speed * 1.05f;
        b->vy = speed * ypact * BALL_ANGVY;
        b->rect.x = game->left.rect.x + game->left.rect.w;
    }
    else if (circle_rect_intersection(*b, game->right.rect))
    {
        float padC  = game->right.rect.y + game->right.rect.h / 2.0f;
        float ballC = game->ball.rect.y + game->ball.r;
        float ypact = 2.0f * (ballC - padC) / game->right.rect.h;   // [-1, 1]
    
        float speed = SDL_fabsf(b->vx);
        b->vx = -(speed * 1.05f);            
        b->vy = speed * ypact * BALL_ANGVY;
        b->rect.x = game->right.rect.x - b->rect.w;
    }

    // score-check
    if (b->rect.x + b->rect.w < 0) // passed left edge
    {
        game->right.score += 1;
        if (game->right.score >= WIN_SCORE) {
            game->right.status = Won;
            game->left.status = Lost;
        }
        reset_ball(game);
    }
    else if (b->rect.x > SCRN_WIDTH) // passed right edge
    {
        game->left.score += 1;
        if (game->left.score >= WIN_SCORE) {
            game->left.status = Won;
            game->right.status = Lost;
        }
        reset_ball(game);
    }
}

void game_over_screen(SDL_Renderer *renderer, char *over_msg, TTF_Font *font)
{
    SDL_Surface *surf  = TTF_RenderText_LCD(font, over_msg, 0, fg, bg);
    if (!surf)
    {
        SDL_Log("Error: %s", SDL_GetError());
        return;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
    if (!tex)
    {
        SDL_Log("Error: %s", SDL_GetError());
        return;
    }
    float w, h;
    SDL_GetTextureSize(tex, &w, &h);
    SDL_FRect dest = { (SCRN_WIDTH - w) / 2, (SCRN_HEIGHT - h) / 2, w, h };
    SDL_RenderTexture(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
}

void display_score(SDL_Renderer *renderer, TTF_Font *font, int score1, int score2)
{
    char buf[32];
    SDL_snprintf(buf, sizeof(buf), "%d - %d", score1, score2);
    SDL_Surface *surf  = TTF_RenderText_LCD(font, buf, 0, fg, bg);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
    if (!tex) return;
    float w, h;
    SDL_GetTextureSize(tex, &w, &h);
    SDL_FRect dest = { (SCRN_WIDTH - w) / 2, 20, w, h };
    SDL_RenderTexture(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
}
