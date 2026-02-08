#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string.h>
#include <math.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

void show_debug_info(const char *msg, int debug)
{
        SDL_Log("%s", msg);
        if (debug)
                SDL_Log(" | %s", SDL_GetError());
}

typedef struct
{
        char name[16]; // 2 bytes
        int mass;      // int will work since it rel mass to mercury
        float radius;
        float distance;
        SDL_Texture *texture;
        SDL_FRect rect;
} Planet;

int main(int argc, char *argv[])
{
        int debug = 0;

        // 2. Check for "-d" flag
        if (argc > 1)
        {
                if (strcmp(argv[1], "-d") == 0)
                {
                        SDL_Log("Debug Mode: Enabled");
                        debug = 1;
                }
        }

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
                show_debug_info("SDL_Init Failed", debug);
                return -1; // -1 = init error
        }

        SDL_Window *pwindow = NULL;
        SDL_Renderer *prenderer = NULL;

        if (!SDL_CreateWindowAndRenderer("Space Sim", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &pwindow, &prenderer))
        {
                show_debug_info("Window/Renderer Failed", debug);
                return -2; // -2 = window/rend error
        }

        int running = 1;

        SDL_Texture *sunTex = IMG_LoadTexture(prenderer, "imgs/assets/sun.png");
        SDL_Texture *earthTex = IMG_LoadTexture(prenderer, "imgs/assets/earth.png");

        if (!sunTex || !earthTex)
        {
                show_debug_info("Failed to Load Textures", debug);
                SDL_DestroyRenderer(prenderer);
                SDL_DestroyWindow(pwindow);
                return -3; // -3 = texture load error
        }

        float sunSize = 100;
        float centerX = SCREEN_WIDTH / 2;
        float centerY = SCREEN_HEIGHT / 2;

        SDL_FRect earthRect = {600, 300, 50, 50};
        SDL_FRect sunRect = {centerX - sunSize / 2, centerY - sunSize / 2, sunSize, sunSize};

        Planet earth = {
            .name = "Earth",
            .mass = 18,
            .radius = 50,
            .distance = 200,
            .texture = earthTex,
            .rect = earthRect,
        };
        float i = 0; // i is angle 
        while (running)
        {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                        if (event.type == SDL_EVENT_QUIT)
                                running = 0;
                }

                SDL_SetRenderDrawColor(prenderer, 0, 0, 0, 255); // black bg
                SDL_RenderClear(prenderer);                      // clear with black

                SDL_RenderTexture(prenderer, sunTex, NULL, &sunRect);

                
                SDL_RenderTexture(prenderer, earthTex, NULL, &earth.rect);

                // earth rotate anim by math sin and cos
                // curr this is fake need to study phy and math more to make it more real
                // to use
                //      Fgi = G * m1 * m2 / r^2
                //      Fei = K * q1 * q2 / r^2
                //      Fi = Fgi + Fei for i th planet by for loop idk
                // todo
                //      make more animation and add more planets
                //      make orbit hints and fix time scale
                //      use real values
                //      add satalaites and sound and muisc
                //      add randome twists like comets and meteors alins and god kannan cameo idk but why 
                earth.rect.x = centerX + earth.distance * sin(i);
                earth.rect.y = centerY + earth.distance * cos(i);
                i += 0.01;
                SDL_RenderPresent(prenderer);
                SDL_Delay(16);
        }

        // Distroy
        SDL_Log("Program Terminated");
        SDL_DestroyTexture(sunTex);
        SDL_DestroyTexture(earthTex);
        SDL_DestroyRenderer(prenderer);
        SDL_DestroyWindow(pwindow);
        SDL_Quit();
        return 0;
}