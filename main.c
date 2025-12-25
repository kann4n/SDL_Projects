#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define SCREEN_WIDTH 900
#define SCREEN_HIGHT 600
#define CELL_SIZE 12
#define CELL_SPACE 1
#define COLs SCREEN_WIDTH / CELL_SIZE
#define ROWs SCREEN_HIGHT / CELL_SIZE

// colors
#define WHITE 0xffffff
#define BLACK 0x000000
#define BLUE 0x0000ff
#define GRAY 0x333333

typedef enum CellType{
    fluid = 'f',
    wall =  'w',
    empty = 'e'
} CellType;

typedef struct Cell
{
    CellType type;
} Cell;


void drawCells(SDL_Surface *surface, Cell cells[COLs][ROWs]){
    for (int i = 0; i < COLs; i++)
    {
        for (int j = 0; j < ROWs; j++)
        {
            switch (cells[i][j].type){
                case fluid:
                    SDL_FillSurfaceRect(surface, &(SDL_Rect){i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE}, BLUE);
                    break;
                case wall:
                    SDL_FillSurfaceRect(surface, &(SDL_Rect){i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE}, WHITE);
                    break;
                case empty:
                    break;
            }
        }
    }
}

void initCells(Cell cells[COLs][ROWs]){
    for (int i = 0; i < COLs; i++)
    {
        for (int j = 0; j < ROWs; j++)
        {
            cells[i][j].type = empty;
        }
    }
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Fluid Simulation", SCREEN_WIDTH, SCREEN_HIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Rect rect = (SDL_Rect){100, 100, 100, 50};
    SDL_Rect rect2 = (SDL_Rect){200, 100, 100, 50};

    uint8_t running = 1;
    SDL_Event event;
    Cell cells[COLs][ROWs];
    CellType currentType = fluid;

    initCells(cells);

    while (running)
    {   
        SDL_FillSurfaceRect(surface, NULL, BLACK); // clear the screen
        
        drawCells(surface, cells); // draw cells
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                uint32_t state = event.motion.state;
                if (state & SDL_BUTTON_LEFT)
                {
                    int x = event.motion.x/CELL_SIZE;
                    int y = event.motion.y/CELL_SIZE;
                    if (x < 0 || x >= COLs || y < 0 || y >= ROWs) continue;
                    cells[x][y].type = currentType;
                }
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                switch (event.key.key)
                {
                case SDLK_E:
                    currentType = empty;
                    break;
                case SDLK_F:
                    currentType = fluid;
                    break;
                case SDLK_W:
                    currentType = wall;
                    break;
                case SDLK_C:
                    initCells(cells); // reset to empty
                    drawCells(surface, cells);
                    SDL_UpdateWindowSurface(window);
                    break;
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                default:
                    printf("Many years ago in a galaxy far, far away...\n");
                    break;
                }
            }
            
            
        }

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16); // max 60 fps cap
    }
}