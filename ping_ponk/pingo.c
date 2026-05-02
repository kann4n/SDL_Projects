#include "game.h"
#include <SDL3_image/SDL_image.h>

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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    const float ballSize = 30;
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
            .r = ballSize/ 2.0f,
            .rect = {SCRN_WIDTH/2.0f - ballSize/2.0f, SCRN_HEIGHT/2.0f - ballSize/2.0f, ballSize, ballSize},
            .vx = BALL_SPEED,
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
                    case SDLK_S:    l_ctrl.move_down = is_down; break;
                    case SDLK_UP:   r_ctrl.move_up   = is_down; break;
                    case SDLK_DOWN: r_ctrl.move_down = is_down; break;
                }
            }
        }

        Uint64 now = SDL_GetTicks();
        float  dt  = (now - last_time) / 1000.0f;
        last_time  = now;
        if (dt > 0.05f) dt = 0.05f;

        bool gameover = game.right.status == Won || game.left.status == Won;
       
        // begin drawing
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, game.left.texture,  NULL, &game.left.rect);
        SDL_RenderTexture(renderer, game.right.texture, NULL, &game.right.rect);
        SDL_RenderTexture(renderer, game.ball.texture, NULL, &game.ball.rect);

        display_score(renderer, font, game.left.score, game.right.score);
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
            SDL_Event e;
            while (SDL_WaitEvent(&e))
            {
                if (e.type == SDL_EVENT_QUIT)       break;
                if (e.type == SDL_EVENT_KEY_DOWN)   break;
            }
            game.running = false;
        }
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
