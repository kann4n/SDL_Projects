#include "kengine.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
        if (!SDL_Init(SDL_INIT_VIDEO))
                return -1;

        SDL_Window *window;
        SDL_Renderer *renderer;
        if (!SDL_CreateWindowAndRenderer("Doom", SCREEN_WID, SCREEN_HIG, 0, &window, &renderer))
                return -2;
        // renderer config
        // set apllha mode on
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // set vsync on/off(if this is on comment out sdl_delay because this func will do it)
        SDL_SetRenderVSync(renderer, 1); // 2nd para = vsync interval

        struct Player player = {150, 150, 5, 3.0f, 0, 0.05f, DEG_TO_RAD(60)};
        struct PlayerControls pcontrol = {0};

        Uint64 lastTime = SDL_GetTicks();
        Uint64 frameKount = 0;
        int running = 1;
        while (running)
        {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                        if (event.type == SDL_EVENT_QUIT)
                                running = 0;

                        int isDown = (event.type == SDL_EVENT_KEY_DOWN); // automatic state finding 0/1
                        if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
                        {
                                switch (event.key.key)
                                {
                                case SDLK_UP:
                                case SDLK_W:
                                        pcontrol.moveForward = isDown;
                                        break;
                                case SDLK_DOWN:
                                case SDLK_S:
                                        pcontrol.moveBack = isDown;
                                        break;
                                case SDLK_LEFT:
                                case SDLK_Q:
                                        pcontrol.rotLeft = isDown;
                                        break;
                                case SDLK_RIGHT:
                                case SDLK_E:
                                        pcontrol.rotRight = isDown;
                                        break;
                                }
                        }
                }

                update_player(&player, pcontrol);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                // Draw Sky
                SDL_SetRenderDrawColor(renderer, 204, 255, 255, 255); // lifht blue sky
                SDL_FRect sky = {0, 0, SCREEN_WID, SCREEN_HIG / 2};
                SDL_RenderFillRect(renderer, &sky);

                // Draw Floor
                SDL_SetRenderDrawColor(renderer, 0, 51, 51, 255); // blueblack floor
                SDL_FRect floor = {0, SCREEN_HIG / 2, SCREEN_WID, SCREEN_HIG / 2};
                SDL_RenderFillRect(renderer, &floor);
                draw_3d(renderer, player);
                draw_map(renderer);
                draw_player(renderer, player);

                SDL_RenderPresent(renderer);

                // other things
                frameKount++;
                Uint64 currentTime = SDL_GetTicks();
                if (currentTime - lastTime > 1000)
                {
                        char title[60];
                        sprintf(title, "fps: %ld", frameKount);
                        SDL_SetWindowTitle(window, title);
                        lastTime = currentTime;
                        frameKount = 0;
                }
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}