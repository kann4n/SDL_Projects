#include "body.h"
#include <math.h>

void update_body(Body* b, float delta_time, float centerX, float centerY) {
    b->angle += b->orbit_speed * delta_time;

    float px = b->parent ? (b->parent->rect.x + b->parent->rect.w / 2.0f) : centerX;
    float py = b->parent ? (b->parent->rect.y + b->parent->rect.h / 2.0f) : centerY;

    b->rect.x = px + b->orbit_radius * cosf(b->angle) - b->rect.w / 2.0f;
    b->rect.y = py + b->orbit_radius * sinf(b->angle) - b->rect.h / 2.0f;
}

void create_orbit_textures(SDL_Renderer* renderer, Body* bodies, int count) {
    for (int i = 0; i < count; i++) {
        if (bodies[i].orbit_radius <= 10) {
            bodies[i].orbit_texture = NULL;
            continue;
        }

        int size = (int)(bodies[i].orbit_radius * 2) + 2;
        bodies[i].orbit_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size, size);
        SDL_SetTextureBlendMode(bodies[i].orbit_texture, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(renderer, bodies[i].orbit_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 150);
        float center = size / 2.0f;
        for (float a = 0; a < 360.0; a += 5.0f) { 
            float x1 = center + bodies[i].orbit_radius * cosf(a * SDL_PI_F / 180.0f);
            float y1 = center + bodies[i].orbit_radius * sinf(a * SDL_PI_F / 180.0f);
            SDL_RenderPoint(renderer, x1, y1);
        }
        SDL_SetRenderTarget(renderer, NULL);
    }
}

void draw_orbit_optimized(SDL_Renderer* renderer, Body* b, float centerX, float centerY) {
    if (!b->orbit_texture) return;
    float px = b->parent ? (b->parent->rect.x + b->parent->rect.w / 2.0f) : centerX;
    float py = b->parent ? (b->parent->rect.y + b->parent->rect.h / 2.0f) : centerY;
    float size = (b->orbit_radius * 2) + 2;
    SDL_FRect dest = { px - b->orbit_radius, py - b->orbit_radius, size, size };
    SDL_RenderTexture(renderer, b->orbit_texture, NULL, &dest);
}

void world_to_screen(float worldX, float worldY, Camera* cam, float* screenX, float* screenY) {
    *screenX = (worldX - cam->x) * cam->zoom + (SCREEN_WIDTH / 2.0f);
    *screenY = (worldY - cam->y) * cam->zoom + (SCREEN_HEIGHT / 2.0f);
}