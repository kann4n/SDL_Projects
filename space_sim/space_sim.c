#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "body.h"
#include <stdio.h>

void cleanup(SDL_Window *win, SDL_Renderer *ren, SDL_Texture **texs, int tex_count)
{
        printf("Destroyed texture ");
        for (int i = 0; i < tex_count; i++)
        {
                if (texs[i]){
                        SDL_DestroyTexture(texs[i]);
                        printf("%d, ", i);
                }
        }
        printf("\n");
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        SDL_Log("Resources cleaned and program terminated.");
}

int main(int argc, char *argv[])
{
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
                SDL_Log("Video Init Error: %s", SDL_GetError());
                return -1;
        }
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if (!SDL_CreateWindowAndRenderer("Space Sim", SCREEN_WIDTH, SCREEN_HEIGHT,
                                         SDL_WINDOW_RESIZABLE, &window, &renderer))
        {
                SDL_Log("Window Init Error: %s", SDL_GetError());
                return -2;
        }
        SDL_SetRenderVSync(renderer, 1); // 1 = On(will cap fps to display refresh rate), 0 = Off
        // Load Textures
        SDL_Texture *tex[BODY_COUNT];
        const char *paths[BODY_COUNT] = {// order must match with BodyTex enum
                                         "../imgs/assets/sun.png",
                                         "../imgs/assets/mercury.png",
                                         "../imgs/assets/venus.png",
                                         "../imgs/assets/earth.png",
                                         "../imgs/assets/moon.png",
                                         "../imgs/assets/mars.png"};
        for (int i = 0; i < BODY_COUNT; i++)
        {
                tex[i] = IMG_LoadTexture(renderer, paths[i]);
                if (!tex[i])
                {
                        SDL_Log("Texture Load Error for %d: %s", i, SDL_GetError());
                        cleanup(window, renderer, tex, BODY_COUNT);
                        return -3;
                }
        }
        Camera main_cam = {.x = 550, .y = 400, .zoom = 1.0f};
        int running = 1;

        Uint64 last_ticks = SDL_GetTicks();
        Uint64 current_ticks;
        float delta = 0.016f;

        Body bodies[BODY_COUNT] = {
            [SUN] = {
                .name = "Sun",
                .orbit_speed = 0.0f,
                .orbit_radius = 0.0f,
                .angle = 0,
                .texture = tex[SUN],
                .rect = {550, 400, 100, 100},
                .parent = NULL},
            [MERCURY] = {.name = "Mercury", .orbit_speed = 1.1f, .orbit_radius = 100.0f, .angle = 0, .texture = tex[MERCURY], .rect = {0, 0, 20, 20}, .parent = &bodies[SUN]},
            [VENUS] = {.name = "Venus", .orbit_speed = 0.8f, .orbit_radius = 150.0f, .angle = 0, .texture = tex[VENUS], .rect = {0, 0, 30, 30}, .parent = &bodies[SUN]},
            [EARTH] = {.name = "Earth", .orbit_speed = 0.6f, .orbit_radius = 250.0f, .angle = 0, .texture = tex[EARTH], .rect = {0, 0, 40, 40}, .parent = &bodies[SUN]},
            [MOON] = {.name = "Moon", .orbit_speed = 2.0f, .orbit_radius = 50.0f, .angle = 0, .texture = tex[MOON], .rect = {0, 0, 10, 10}, .parent = &bodies[EARTH]},
            [MARS] = {.name = "Mars", .orbit_speed = 0.5f, .orbit_radius = 350.0f, .angle = 0, .texture = tex[MARS], .rect = {0, 0, 40, 40}, .parent = &bodies[SUN]}};

        float planetCenterPoint[2] = {
            (bodies[SUN].rect.x + bodies[SUN].rect.w / 2.0f),
            (bodies[SUN].rect.y + bodies[SUN].rect.h / 2.0f)};
        // // for fps calc
        // Uint64 fps_last_time = SDL_GetTicks();
        // int frame_count = 0;
        // char title_buffer[64];
        // Pre-create orbit textures if enabled
        if (DRAW_ORBITS)
        {
                create_orbit_textures(renderer, bodies, BODY_COUNT);
        }
        while (running)
        {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                        if (event.type == SDL_EVENT_QUIT)
                                running = 0;

                        if (event.type == SDL_EVENT_MOUSE_WHEEL)
                        {
                                if (event.wheel.y > 0)
                                        main_cam.zoom *= 1.01f; // Zoom in
                                else if (event.wheel.y < 0)
                                        main_cam.zoom *= 0.99f; // Zoom out
                        }
                }

                // Delta Time calculation
                current_ticks = SDL_GetTicks();
                delta = (current_ticks - last_ticks) / 1000.0f;
                last_ticks = current_ticks;

                // Update Physics
                for (int i = 0; i < BODY_COUNT; i++)
                {
                        update_body(&bodies[i], delta, planetCenterPoint[0], planetCenterPoint[1]);
                }

                // Render
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Deep space clr
                SDL_RenderClear(renderer);

                if (DRAW_ORBITS)
                {
                        for (int i = 0; i < BODY_COUNT; i++)
                        {
                                draw_orbit_optimized(renderer, &bodies[i], planetCenterPoint[0], planetCenterPoint[1]);
                        }
                }
                for (int i = 0; i < BODY_COUNT; i++)
                {
                        float sX, sY;
                        // Calculate where the body should be on screen based on camera
                        world_to_screen(bodies[i].rect.x, bodies[i].rect.y, &main_cam, &sX, &sY);

                        SDL_FRect draw_rect = {
                            sX,
                            sY,
                            bodies[i].rect.w * main_cam.zoom, // Scale size by zoom
                            bodies[i].rect.h * main_cam.zoom};

                        SDL_RenderTextureRotated(renderer, bodies[i].texture, NULL, &draw_rect,
                                                 bodies[i].angle * 0.5f, NULL, SDL_FLIP_NONE);
                }

                SDL_RenderPresent(renderer);
        }

        cleanup(window, renderer, tex, BODY_COUNT);
        return 0;
}