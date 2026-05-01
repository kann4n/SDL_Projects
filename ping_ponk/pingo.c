#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>

#define SCRN_WIDTH  800
#define SCRN_HEIGHT 600
#define BALL_SPEED  200.0f
#define SPEED       400.0f
#define WIN_SCORE   2
#define PLYR_WIDTH  10
#define PLYR_HEIGHT 50
#define PLYR_OFFSET 10

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
    float         x, y, r;
    SDL_Texture  *texture;
    float         vx, vy;
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

void reset_ball(Game *game)
{
    game->ball.x  = (float)SCRN_WIDTH / 2;
    game->ball.y  = (float)SCRN_HEIGHT / 2;
    game->ball.vx = (SDL_rand(2) == 0 ? 1 : -1) * BALL_SPEED;
    game->ball.vy = (SDL_rand(2) == 0 ? 1 : -1) * BALL_SPEED;
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
    b->x += b->vx * dt;
    b->y += b->vy * dt;

    if (b->y - b->r < 0 || b->y + b->r > SCRN_HEIGHT)
    {
        b->vy = -b->vy;
        b->y  = SDL_clamp(b->y, b->r, SCRN_HEIGHT - b->r);
    }

    bool      isatleft = b->x < (float)SCRN_WIDTH / 2;
    SDL_FRect ballrect = { b->x - b->r, b->y - b->r, b->r * 2, b->r * 2 };

    if (isatleft)
    {
        if (SDL_HasRectIntersectionFloat(&ballrect, &game->left.rect))
        {
            b->vx  = -b->vx;
            b->x   = game->left.rect.x + game->left.rect.w + b->r;
            b->vx *= 1.001f;
        }
        else if (b->x + b->r < 0)
        {
            reset_ball(game);
            game->right.score += 1;
            if (game->right.score >= WIN_SCORE)
            {
                game->right.status = Won;
                game->left.status  = Lost;
            }
        }
    }
    else
    {
        if (SDL_HasRectIntersectionFloat(&ballrect, &game->right.rect))
        {
            b->vx  = -b->vx;
            b->x   = game->right.rect.x - b->r;
            b->vx *= 1.001f;
        }
        else if (b->x > SCRN_WIDTH)
        {
            reset_ball(game);
            game->left.score += 1;
            if (game->left.score >= WIN_SCORE)
            {
                game->left.status  = Won;
                game->right.status = Lost;
            }
        }
    }
}

void game_over_screen(SDL_Renderer *renderer, char *over_msg, TTF_Font *font)
{
    SDL_Color    color = { 255, 255, 255, 255 };
    SDL_Surface *surf  = TTF_RenderText_Blended(font, over_msg, 0, color);
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
    SDL_FRect clr_dest = {dest.x, dest.y, dest.w, dest.h};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &clr_dest);
    SDL_RenderTexture(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
}

void display_score(SDL_Renderer *renderer, TTF_Font *font, int score1, int score2)
{
    char buf[32];
    SDL_snprintf(buf, sizeof(buf), "%d - %d", score1, score2);
    SDL_Color    white = { 255, 255, 255, 255 };
    SDL_Surface *surf  = TTF_RenderText_Blended(font, buf, 0, white);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
    if (!tex) return;
    float w, h;
    SDL_GetTextureSize(tex, &w, &h);
    SDL_FRect dest = { (SCRN_WIDTH - w) / 2, 20, w, h };
    SDL_FRect clr_dest = {dest.x, dest.y, dest.w, dest.h};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &clr_dest);
    SDL_RenderTexture(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    if (!TTF_Init())
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_srand(SDL_GetTicks());
    if (!SDL_CreateWindowAndRenderer("Pingo", SCRN_WIDTH, SCRN_HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("Failed to create window/renderer: %s", SDL_GetError());
        return -1;
    }

    Game game = {
        .left  = {
            .rect = { PLYR_OFFSET, SCRN_HEIGHT * 0.25f, PLYR_WIDTH, PLYR_HEIGHT },
            .texture = IMG_LoadTexture(renderer, "assets/boxer.png"),
            .status = Ready,
            .score = 0
        },
        .right = {
            .rect = { SCRN_WIDTH - PLYR_WIDTH - PLYR_OFFSET, SCRN_HEIGHT * 0.75f, PLYR_WIDTH, PLYR_HEIGHT },
            .texture = IMG_LoadTexture(renderer, "assets/boxer.png"),
            .status = Ready,
            .score = 0
        },
        .ball  = {
            .x = (float)SCRN_WIDTH / 2,
            .y = (float)SCRN_HEIGHT / 2,
            .r = 15, .vx = BALL_SPEED,
            .vy = BALL_SPEED * 0.5f,
            .texture = IMG_LoadTexture(renderer, "assets/red-ball.png")
        },
        .running = true,
    };

    if (!game.left.texture || !game.right.texture || !game.ball.texture)
    {
        SDL_Log("Failed to load textures: %s", SDL_GetError());
        return -2;
    }

    TTF_Font *font = TTF_OpenFont("assets/Tiny5-Regular.ttf", 32);
    if (!font)
    {
        SDL_Log("Font load failed: %s", SDL_GetError());
        SDL_DestroyTexture(game.left.texture);
        SDL_DestroyTexture(game.right.texture);
        SDL_DestroyTexture(game.ball.texture);
        return -3;
    }

    PlayerControls l_ctrl = {0}, r_ctrl = {0};
    Uint64         last_time = SDL_GetTicks();

    while (game.running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                game.running = false;

            if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
            {
                bool is_down = (event.type == SDL_EVENT_KEY_DOWN);
                switch (event.key.key)
                {
                    case SDLK_W:    l_ctrl.move_up   = is_down; break;
                    case SDLK_S:    l_ctrl.move_down  = is_down; break;
                    case SDLK_UP:   r_ctrl.move_up   = is_down; break;
                    case SDLK_DOWN: r_ctrl.move_down  = is_down; break;
                }
            }
        }

        Uint64 now = SDL_GetTicks();
        float  dt  = (now - last_time) / 1000.0f;
        last_time  = now;
        if (dt > 0.05f) dt = 0.05f;

        bool gameover = game.right.status == Won || game.left.status == Won;
        if (!gameover)
        {
            update_player(&game.left,  l_ctrl.move_up, l_ctrl.move_down, dt);
            update_player(&game.right, r_ctrl.move_up, r_ctrl.move_down, dt);
            update_ball(&game, dt);
        }
        else
        {
            char *gameover_msg = (game.left.status == Won) ? "Left PLAYER WON" : "RIGHT PLAYER WON";
            game_over_screen(renderer, gameover_msg, font);
            display_score(renderer, font, game.left.score, game.right.score);
            SDL_RenderPresent(renderer);
            SDL_Delay(1500);
            SDL_Event e;
            while (SDL_WaitEvent(&e))
            {
                if (e.type == SDL_EVENT_QUIT)       break;
                if (e.type == SDL_EVENT_KEY_DOWN)   break;
            }
            game.running = false;
        }

        // begin drawing
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, game.left.texture,  NULL, &game.left.rect);
        SDL_RenderTexture(renderer, game.right.texture, NULL, &game.right.rect);

        SDL_FRect b_rect = {
            game.ball.x - game.ball.r,
            game.ball.y - game.ball.r,
            game.ball.r * 2.0f,
            game.ball.r * 2.0f,
        };
        SDL_RenderTexture(renderer, game.ball.texture, NULL, &b_rect);
        display_score(renderer, font, game.left.score, game.right.score);
        // end drawing
        
        // update screen
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(game.left.texture);
    SDL_DestroyTexture(game.right.texture);
    SDL_DestroyTexture(game.ball.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
