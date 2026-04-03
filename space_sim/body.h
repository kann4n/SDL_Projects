#ifndef BODY_H
#define BODY_H

#include <SDL3/SDL.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define DRAW_ORBITS 0 // Set to 0 for no orbits

// grp of textures for body 
typedef enum BodyTex {
    SUN, 
    MERCURY, 
    VENUS, 
    EARTH, 
    MOON, 
    MARS, 
    BODY_COUNT
} BodyTex;

typedef struct Body {
    const char* name;
    float orbit_speed;
    float orbit_radius;
    float angle;
    SDL_Texture* texture;
    SDL_Texture* orbit_texture;
    SDL_FRect rect;
    struct Body* parent;
} Body;

typedef struct {
    float x;     // Center of the camera in the world
    float y;
    float zoom;  // 1.0f is normal, 2.0f is zoomed in, 0.5f is zoomed out
} Camera;

void update_body(Body* b, float delta_time, float centerX, float centerY);
void create_orbit_textures(SDL_Renderer* renderer, Body* bodies, int count);
void draw_orbit_optimized(SDL_Renderer* renderer, Body* b, float centerX, float centerY);
void world_to_screen(float worldX, float worldY, Camera* cam, float* screenX, float* screenY);

#endif