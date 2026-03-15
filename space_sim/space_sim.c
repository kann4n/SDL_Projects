#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

typedef enum BodyTex {
        SUN,
        MERCURY,
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

void draw_orbit(SDL_Renderer* renderer, Body* b, float centerX, float centerY) {
        if (b->orbit_radius <= 10) return; // Sun doesn't need an orbit line

        float px = b->parent ? (b->parent->rect.x + b->parent->rect.w / 2.0f) : centerX;
        float py = b->parent ? (b->parent->rect.y + b->parent->rect.h / 2.0f) : centerY;

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100); //grey
        for (int i = 0; i < 360; i += 1) { // just hint 
                float x = px + b->orbit_radius * cosf(i * SDL_PI_F / 180.0f);
                float y = py + b->orbit_radius * sinf(i * SDL_PI_F / 180.0f);
                float x2 = px + b->orbit_radius * cosf((i + 1) * SDL_PI_F / 180.0f);
                float y2 = py + b->orbit_radius * sinf((i + 1) * SDL_PI_F / 180.0f);
                SDL_RenderLine(renderer, x, y, x2, y2);
        }
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
        SDL_SetRenderVSync(renderer, 1); // 1 = On, 0 = Off
        // Load Textures
        SDL_Texture* tex[BODY_COUNT];
        tex[SUN] = IMG_LoadTexture(renderer, "../imgs/assets/sun.png");
        tex[MERCURY] = IMG_LoadTexture(renderer, "../imgs/assets/mercury.png");
        tex[EARTH] = IMG_LoadTexture(renderer, "../imgs/assets/earth.png");
        tex[MOON] = IMG_LoadTexture(renderer, "../imgs/assets/moon.jpg");

        if (!tex[0] || !tex[1] || !tex[2]) {
                SDL_Log("Texture Error: %s", SDL_GetError());
                cleanup(window, renderer, tex, 3);
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
                [EARTH] = {
                        .name = "Earth",
                        .orbit_speed = 0.9f,
                        .orbit_radius = 200.0f,
                        .angle = 0,
                        .texture = tex[EARTH],
                        .rect = {0, 0, 50, 50},
                        .parent = &bodies[SUN]
                },
                [MOON] = {
                        .name = "Moon",
                        .orbit_speed = 2.0f,
                        .orbit_radius = 50.0f,
                        .angle = 0,
                        .texture = tex[MOON],
                        .rect = {0, 0, 20, 20},
                        .parent = &bodies[EARTH]
                }
        };

        float planetCenterPoint[2] = {
                (bodies[SUN].rect.x + bodies[SUN].rect.w / 2.0f),
                 (bodies[SUN].rect.y + bodies[SUN].rect.h / 2.0f)
        };

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

                for (int i = 0; i < BODY_COUNT; i++) {
                        draw_orbit(renderer, &bodies[i], planetCenterPoint[0], planetCenterPoint[1]);
                }
                for (int i = 0; i < BODY_COUNT; i++) {
                        SDL_RenderTextureRotated(renderer, bodies[i].texture, NULL, &bodies[i].rect,
                                bodies[i].angle * 0.5f, NULL, SDL_FLIP_NONE);
                }

                SDL_RenderPresent(renderer);
        }

        cleanup(window, renderer, tex, BODY_COUNT);
        return 0;
}