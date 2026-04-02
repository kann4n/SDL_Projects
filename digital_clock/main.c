#include "clock.h"

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Video Init Error: %s", SDL_GetError());
        return -1;
    }
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!SDL_CreateWindowAndRenderer("Digital Clock", SCREEN_WID, SCREEN_HIG, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Window Init Error: %s", SDL_GetError());
        return -2;
    }

    int running = 1;
    float x1 = (SEGMENT_LEN * 3 + SEGMENT_WID * 2 + 5 + SEGMENT_LEN * 6 - 5) / 2;
    float x2 = (SEGMENT_LEN * 7 + SEGMENT_WID * 2 + 5 + SEGMENT_LEN * 10 - 5) / 2;
    SDL_FPoint colons[2] = {{x1, SCREEN_HIG / 2.0f}, {x2, SCREEN_HIG / 2.0f}};
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = 0;
        }
        // clear with black color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        display_clock(renderer);
        display_colon(renderer, colons);

        // present the final output
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 60 FPS
    }

    // distroy and quit sdl
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
