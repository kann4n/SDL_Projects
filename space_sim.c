#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 720

typedef enum BodyTex {
        SUN,
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

int main(int argc, char* argv[])
{
        if (!SDL_Init(SDL_INIT_VIDEO))
                return -1;

        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (!SDL_CreateWindowAndRenderer("Space Sim", SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_MAXIMIZED  , &window, &renderer)) {
                SDL_Log("Window Error: %s", SDL_GetError());
                return -2;
        }

        // Load Textures
        SDL_Texture* tex[BODY_COUNT];
        tex[SUN] = IMG_LoadTexture(renderer, "imgs/assets/sun.png");
        tex[EARTH] = IMG_LoadTexture(renderer, "imgs/assets/earth.png");
        tex[MOON] = IMG_LoadTexture(renderer, "imgs/assets/moon.jpg");

        if (!tex[0] || !tex[1] || !tex[2]) {
                SDL_Log("Texture Error: %s", SDL_GetError());
                cleanup(window, renderer, tex, 3);
                return -3;
        }

        Body sun = {
            .name = "Sun",
            .orbit_speed = 0.0f,
            .orbit_radius = 0,
            .angle = 0,
            .texture = tex[SUN], // sun Texture
            .rect = { SCREEN_WIDTH / 2.0f - 100 / 2, SCREEN_HEIGHT / 2.0f - 100 / 2, 100, 100 },
            .parent = NULL
        };

        Body earth = {
            .name = "Earth",
            .orbit_speed = 0.5f,
            .orbit_radius = 200.0f,
            .angle = 0,
            .texture = tex[EARTH], // earth Texture
            .rect = { 0, 0, 50, 50 }, // will be done via update call
            .parent = NULL
        };

        Body moon = {
            .name = "Moon",
            .orbit_speed = 2.0f,
            .orbit_radius = 60.0f,
            .angle = 0,
            .texture = tex[MOON],
            .rect = { 0, 0, 20, 20 }, // will be done via update call
            .parent = &earth
        };

        int running = 1;
        Uint64 last_ticks = SDL_GetTicks();

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
                update_body(&earth, delta, sun.rect.x + sun.rect.w / 2.0f, sun.rect.y + sun.rect.h / 2.0f);
                update_body(&moon, delta, 0, 0); // Center is ignored because moon has a parent

                // Render
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 5, 5, 15, 20); // Deep space blue/black
                SDL_RenderFillRect(renderer, NULL);

                SDL_RenderTexture(renderer, sun.texture, NULL, &sun.rect);
                SDL_RenderTexture(renderer, earth.texture, NULL, &earth.rect);
                SDL_RenderTexture(renderer, moon.texture, NULL, &moon.rect);

                SDL_RenderPresent(renderer);
        }

        cleanup(window, renderer, tex, BODY_COUNT);
        return 0;
}