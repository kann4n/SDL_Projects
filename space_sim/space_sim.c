#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define DRAW_ORBITS 1 // Set to 0 for no orbits

typedef enum BodyTex {
        SUN,
        MERCURY,
        VENUS,
        EARTH,
        MOON,
        BODY_COUNT // tip it say the count without counting
} BodyTex;

typedef struct Body {
        const char* name;
        float orbit_speed;
        float orbit_radius;
        float angle;
        SDL_Texture* texture;
        SDL_Texture* orbit_texture;
        SDL_FRect rect;
        struct Body* parent; // Orbit around this body
} Body;

// Helper to update body position based on time and parent
void update_body(Body* b, float delta_time, float centerX, float centerY)
{
        b->angle += b->orbit_speed * delta_time;

        float px
                = b->parent ? (b->parent->rect.x + b->parent->rect.w / 2.0f) : centerX;
        float py
                = b->parent ? (b->parent->rect.y + b->parent->rect.h / 2.0f) : centerY;

        b->rect.x = px + b->orbit_radius * cosf(b->angle) - b->rect.w / 2.0f;
        b->rect.y = py + b->orbit_radius * sinf(b->angle) - b->rect.h / 2.0f;
}

void create_orbit_textures(SDL_Renderer* renderer, Body* bodies, int count) {
        for (int i = 0; i < count; i++) {
                if (bodies[i].orbit_radius <= 10) {
                        bodies[i].orbit_texture = NULL;
                        continue;
                }

                // 1. Create a transparent texture twice the size of the radius
                int size = (int)(bodies[i].orbit_radius * 2) + 2;
                bodies[i].orbit_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, size, size);

                SDL_SetTextureBlendMode(bodies[i].orbit_texture, SDL_BLENDMODE_BLEND);

                // 2. Switch rendering to this texture
                SDL_SetRenderTarget(renderer, bodies[i].orbit_texture);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Transparent background
                SDL_RenderClear(renderer);

                // 3. Draw the orbit circle into the texture
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 150);
                float center = size / 2.0f;
                for (float a = 0; a < 360.0; a += 5.0f) { 
                        float x1 = center + bodies[i].orbit_radius * cosf(a * SDL_PI_F / 180.0f);
                        float y1 = center + bodies[i].orbit_radius * sinf(a * SDL_PI_F / 180.0f);
                        SDL_RenderPoint(renderer, x1, y1);
                }

                // 4. Reset renderer back to the window
                SDL_SetRenderTarget(renderer, NULL);
        }
}

void cleanup(SDL_Window* win, SDL_Renderer* ren, SDL_Texture** texs, int tex_count)
{
        for (int i = 0; i < tex_count; i++) {
                if (texs[i])
                        SDL_DestroyTexture(texs[i]);
        }
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        SDL_Log("Resources cleaned and program terminated.");
}

void draw_orbit_optimized(SDL_Renderer* renderer, Body* b, float centerX, float centerY) {
        if (!b->orbit_texture) return;

        float px = b->parent ? (b->parent->rect.x + b->parent->rect.w / 2.0f) : centerX;
        float py = b->parent ? (b->parent->rect.y + b->parent->rect.h / 2.0f) : centerY;

        float size = (b->orbit_radius * 2) + 2;
        SDL_FRect dest = { px - b->orbit_radius, py - b->orbit_radius, size, size };

        SDL_RenderTexture(renderer, b->orbit_texture, NULL, &dest);
}

int main(int argc, char* argv[])
{
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
                SDL_Log("Video Init Error: %s", SDL_GetError());
                return -1;
        }
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (!SDL_CreateWindowAndRenderer("Space Sim", SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_MAXIMIZED, &window, &renderer)) {
                SDL_Log("Window Init Error: %s", SDL_GetError());
                return -2;
        }
        SDL_SetRenderVSync(renderer, 1); // 1 = On(will cap fps to display refresh rate), 0 = Off
        // Load Textures
        SDL_Texture* tex[BODY_COUNT];
        tex[SUN] = IMG_LoadTexture(renderer, "../imgs/assets/sun.png");
        tex[MERCURY] = IMG_LoadTexture(renderer, "../imgs/assets/mercury.png");
        tex[VENUS] = IMG_LoadTexture(renderer, "../imgs/assets/venus.png");
        tex[EARTH] = IMG_LoadTexture(renderer, "../imgs/assets/earth.png");
        tex[MOON] = IMG_LoadTexture(renderer, "../imgs/assets/moon.png");

        if (!tex[0] || !tex[1] || !tex[2] || !tex[3] || !tex[4]) {
                SDL_Log("Texture Error: %s", SDL_GetError());
                cleanup(window, renderer, tex, BODY_COUNT);
                return -3;
        }

        int running = 1;
        Uint64 last_ticks = SDL_GetTicks();
        Body bodies[BODY_COUNT] = {
                [SUN] = {
                       .name = "Sun",
                       .orbit_speed = 0.0f,
                       .orbit_radius = 0.0f,
                       .angle = 0,
                       .texture = tex[SUN],
                       .rect = {550, 400, 100, 100},
                       .parent = NULL
                },
                [MERCURY] = {
                        .name = "Mercury",
                        .orbit_speed = 1.1f,
                        .orbit_radius = 100.0f,
                        .angle = 0,
                        .texture = tex[MERCURY],
                        .rect = {0, 0, 20, 20},
                        .parent = &bodies[SUN]
                },
                [VENUS] = {
                        .name = "Venus",
                        .orbit_speed = 0.8f,
                        .orbit_radius = 150.0f,
                        .angle = 0,
                        .texture = tex[VENUS],
                        .rect = {0, 0, 30, 30},
                        .parent = &bodies[SUN]
                },
                [EARTH] = {
                        .name = "Earth",
                        .orbit_speed = 0.6f,
                        .orbit_radius = 250.0f,
                        .angle = 0,
                        .texture = tex[EARTH],
                        .rect = {0, 0, 40, 40},
                        .parent = &bodies[SUN]
                },
                [MOON] = {
                        .name = "Moon",
                        .orbit_speed = 2.0f,
                        .orbit_radius = 50.0f,
                        .angle = 0,
                        .texture = tex[MOON],
                        .rect = {0, 0, 10, 10},
                        .parent = &bodies[EARTH]
                }
        };

        float planetCenterPoint[2] = {
                (bodies[SUN].rect.x + bodies[SUN].rect.w / 2.0f),
                 (bodies[SUN].rect.y + bodies[SUN].rect.h / 2.0f)
        };
        // // for fps calc
        // Uint64 fps_last_time = SDL_GetTicks();
        // int frame_count = 0;
        // char title_buffer[64];
        // Pre-create orbit textures if enabled
        if (DRAW_ORBITS) {
                create_orbit_textures(renderer, bodies, BODY_COUNT);
        }
        while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_EVENT_QUIT)
                                running = 0;
                }

                // Delta Time calculation
                Uint64 current_ticks = SDL_GetTicks();
                float delta = (current_ticks - last_ticks) / 1000.0f;
                last_ticks = current_ticks;


                // Update Physics
                for (int i = 0; i < BODY_COUNT; i++) {
                        update_body(&bodies[i], delta, planetCenterPoint[0], planetCenterPoint[1]);
                }

                // Render
                SDL_SetRenderDrawColor(renderer, 5, 5, 20, 255); // Deep space clr
                SDL_RenderClear(renderer);

                if (DRAW_ORBITS)
                {
                        for (int i = 0; i < BODY_COUNT; i++) {
                                draw_orbit_optimized(renderer, &bodies[i], planetCenterPoint[0], planetCenterPoint[1]);
                        }
                }
                for (int i = 0; i < BODY_COUNT; i++) {
                        SDL_RenderTextureRotated(renderer, bodies[i].texture, NULL, &bodies[i].rect,
                                bodies[i].angle * 0.5f, NULL, SDL_FLIP_NONE);
                }

                SDL_RenderPresent(renderer);
                // frame_count++;
                // Uint64 fps_current_time = SDL_GetTicks();
                // if (fps_current_time - fps_last_time >= 1000) {
                //         float fps = frame_count / ((fps_current_time - fps_last_time) / 1000.0f);
                //         sprintf(title_buffer, "Space Sim - FPS: %.2f", fps);
                //         SDL_SetWindowTitle(window, title_buffer);

                //         fps_last_time = fps_current_time;
                //         frame_count = 0;
                // }
        }

        cleanup(window, renderer, tex, BODY_COUNT);
        return 0;
}