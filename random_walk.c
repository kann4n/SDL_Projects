#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#define SCREEN_WIDTH 900
#define SCREEN_HIGHT 600
#define R_WIDTH 2
#define R_HEIGHT 2
#define SCALE 10
// if scale > size it make dotted random walk

typedef struct Direction
{
    int dir[2];
} direction;

typedef struct Agent
{
    SDL_Rect rect;
    Uint32 color;
} Agent;

void HSLtoRGB(float h, float s, float l, Uint8 *r, Uint8 *g, Uint8 *b)
{
    // credit: https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsl-and-hsl-to-rgb
    float c = (1.0 - fabs(2.0 * l - 1.0)) * s;
    float x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
    float m = l - c / 2.0;

    float rp, gp, bp;
    if (h >= 0 && h < 60)       { rp = c; gp = x; bp = 0; }
    else if (h >= 60 && h < 120){ rp = x; gp = c; bp = 0; }
    else if (h >= 120 && h < 180){ rp = 0; gp = c; bp = x; }
    else if (h >= 180 && h < 240){ rp = 0; gp = x; bp = c; }
    else if (h >= 240 && h < 300){ rp = x; gp = 0; bp = c; }
    else                        { rp = c; gp = 0; bp = x; }

    *r = (Uint8)((rp + m) * 255);
    *g = (Uint8)((gp + m) * 255);
    *b = (Uint8)((bp + m) * 255);
}

void changeXY(SDL_Rect *rect, direction d)
{
    rect->x += d.dir[0];
    rect->y += d.dir[1];
    // check boundarys
    if (rect->x < 0)
        rect->x = 0;
    else if (rect->x > SCREEN_WIDTH)
        rect->x = SCREEN_WIDTH;

    if (rect->y < 0)
        rect->y = 0;
    else if (rect->y > SCREEN_HIGHT)
        rect->y = SCREEN_HIGHT;
}

void Rand4Move(SDL_Rect *rect, int dir)
{
    direction up = {{0, -1}};
    direction right = {{1, 0}};
    direction down = {{0, 1}};
    direction left = {{-1, 0}};
    // rect is alredy a pointer
    switch (dir)
    {
    case 0: // up
        changeXY(rect, up);
        break;
    case 1: // right
        changeXY(rect, right);
        break;
    case 2: // down
        changeXY(rect, down);
        break;
    case 3: // left
        changeXY(rect, left);
        break;
    default:
        printf("Not vaild dir"); // log error
        break;
    }
}

int main(int argc, const char *argv[]) // argc = arg count, argv = arg vector
{
    int num_of_agent = 5;
    if (argc == 2)
    {
        num_of_agent = atoi(argv[1]);
    }
    else if (argc >= 2)
    {
        printf("Usage : <num_of_agent>\n");
        return 1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }

    SDL_Window *pwindow = SDL_CreateWindow("Random Walk", SCREEN_WIDTH, SCREEN_HIGHT, 0);
    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);
    const SDL_PixelFormatDetails *Pixformat = SDL_GetPixelFormatDetails(psurface->format);

    Agent *agents = malloc(sizeof(SDL_Rect) * num_of_agent);
    for (int i = 0; i < num_of_agent; i++)
    {
        agents[i].rect.x = SCREEN_WIDTH / 2;
        agents[i].rect.y = SCREEN_HIGHT / 2;
        agents[i].rect.w = R_WIDTH;
        agents[i].rect.h = R_HEIGHT;
        
        float h = (float) (360/num_of_agent) * i;
        Uint8 r, g, b;
        HSLtoRGB(h, 1.0f, 0.5f, &r, &g, &b);

        // Convert separate r,g,b into a format SDL understands for this surface
        agents[i].color = SDL_MapRGB(Pixformat,NULL, r, g, b);
    }

    srand(time(NULL));

    int running = 1;
    int canFreelyMove = 0; // no you can't
    int only4Dir = 1;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
        }
        for (int i = 0; i < num_of_agent; i++)
        {
            if (canFreelyMove)
            {
                int xdir = (rand() % 3) - 1; // rand % 3 may return 0,1,2 and -1 make it -1,0,1
                int ydir = (rand() % 3) - 1; // rand % 3 may return 0,1,2 and -1 make it -1,0,1
                for (int j = 0; j < SCALE; j++)
                {
                    agents[i].rect.x += xdir;
                    agents[i].rect.y += ydir;
                }
            }
            else if (only4Dir)
            {
                int dir = rand() % 4; // dir = 0, 1, 2, 3

                for (int j = 0; j < SCALE; j++)
                {
                    Rand4Move(&agents[i].rect, dir);
                    SDL_FillSurfaceRect(psurface, &agents[i].rect, agents[i].color);
                }
            }
        }
        SDL_UpdateWindowSurface(pwindow);
        SDL_Delay(18); // ~50-60 fps
    }
    free(agents);
    SDL_Quit();
    return 0;
}
