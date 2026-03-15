#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define SCREEN_WIDTH 900
#define SCREEN_HIGHT 600
#define CELL_SIZE 10
#define COLs (SCREEN_WIDTH / CELL_SIZE)
#define ROWs (SCREEN_HIGHT / CELL_SIZE)

typedef enum CellType
{
    fluid = 'f',
    wall = 'w',
    empty = 'e'
} CellType;

typedef struct Cell
{
    CellType type;
} Cell;

void initCells(Cell cells[COLs][ROWs]);
void drawCells(SDL_Renderer *renderer, Cell cells[COLs][ROWs]);
void updateCells(Cell cells[COLs][ROWs]);

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Fluid Simulation", SCREEN_WIDTH, SCREEN_HIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    uint8_t running = 1;
    SDL_Event event;
    Cell cells[COLs][ROWs];
    CellType currentType = fluid;

    initCells(cells);

    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        drawCells(renderer, cells);
        updateCells(cells);
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
                    int x = (int)(event.motion.x / CELL_SIZE);
                    int y = (int)(event.motion.y / CELL_SIZE);

                    if (x >= 0 && x < COLs && y >= 0 && y < ROWs)
                    {
                        cells[x][y].type = currentType;
                    }
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
                    initCells(cells);
                    break;
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // max 60 fps cap
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void initCells(Cell cells[COLs][ROWs])
{
    for (int i = 0; i < COLs; i++)
    {
        for (int j = 0; j < ROWs; j++)
        {
            cells[i][j].type = empty;
        }
    }
}

void drawCells(SDL_Renderer *renderer, Cell cells[COLs][ROWs])
{
    for (int i = 0; i < COLs; i++)
    {
        for (int j = 0; j < ROWs; j++)
        {
            if (cells[i][j].type == empty)
                continue;
            switch (cells[i][j].type)
            {
            case fluid:
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
                break;
            case wall:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
                break;
            case empty:
                break;
            }
            SDL_FRect rect = {(float)(i * CELL_SIZE), (float)(j * CELL_SIZE), CELL_SIZE, CELL_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void updateCells(Cell cells[COLs][ROWs])
{
    for (int i = COLs - 1; i >= 0; i--)
    {
        for (int j = ROWs - 1; j >= 0; j--)
        {
            if (cells[i][j].type == fluid)
            {
                // check below
                if (j + 1 < ROWs)
                {
                    Cell *below = &cells[i][j + 1]; // Pointer to the cell below
                    if (below->type == empty)
                    {
                        // Move fluid down
                        below->type = fluid;
                        cells[i][j].type = empty;
                        continue;
                    }
                }
                
            }
        }
    }
}