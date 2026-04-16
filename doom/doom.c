#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WID 500
#define SCREEN_HIG 500
#define MAP_XB 24
#define MAP_YB 24
#define TILE_SIZE 50

// helpers
#define M_PI 3.14159265358979323846
#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0f)

int map[MAP_YB][MAP_XB] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

struct Player
{
        float x, y;
        float size;
        float speed;
        float angle;
        float rotSpeed; // in rad
        float fov;      // in rad
};

struct PlayerControls
{
        int moveForward, moveBack;
        int moveLeft, moveRight;
        int rotLeft, rotRight;
};

void draw_map(SDL_Renderer *renderer)
{
        for (int i = 0; i < MAP_YB; i++)
        {
                for (int j = 0; j < MAP_XB; j++)
                {
                        if (map[i][j] != 0)
                        {
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_FRect block = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE - 1, TILE_SIZE - 1};
                                SDL_RenderFillRect(renderer, &block);
                        }
                }
        }
}

void draw_player(SDL_Renderer *renderer, struct Player player)
{
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_FRect player_obj = {player.x, player.y, player.size, player.size};
        SDL_RenderFillRect(renderer, &player_obj);
        float endX = player_obj.x + 100 * cosf(player.angle);
        float endY = player_obj.y + 100 * sinf(player.angle);
        SDL_RenderLine(renderer, player_obj.x, player_obj.y, endX, endY);
}

/*#########################| easy to understand but very ineffecint |#########################|*/
// void draw_rays(SDL_Renderer *renderer, struct Player player)
// {
//         int RAYS = 100;
//         float start_angle = player.angle - (player.fov / 2);
//         float angle_step = player.fov / (float)RAYS;
//         for (int i = 0; i < RAYS; i++)
//         {
//                 float ray_angle = start_angle + (i * angle_step);
//                 float kos = cosf(ray_angle);
//                 float kin = sinf(ray_angle);
//                 float distence = 0;
//                 int hit = 0;
//                 while(!hit && distence < 700){
//                         distence += 1; // small is good
//                         int rayhitX = (int)(player.x + kos*distence)/TILE_SIZE;
//                         int rayhitY = (int)(player.y + kin*distence)/TILE_SIZE;
//                         if(rayhitX < 0 || rayhitX >= MAP_XB-1 || rayhitY < 0 || rayhitY >= MAP_YB-1){
//                                 hit = 1;
//                         }else if (map[rayhitY][rayhitX] > 0){
//                                 hit = 1;
//                         }
//                 }
//                 SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
//                 SDL_RenderLine(renderer, player.x, player.y, player.x + kos*distence, player.y + kin*distence);
//         }
// }

void draw_rays(SDL_Renderer *renderer, struct Player player)
{
        const int RAYS = 100;
        float start_angle = player.angle - (player.fov / 2.0f);
        float angle_step = player.fov / (float)RAYS;

        for (int i = 0; i < RAYS; i++)
        {
                float ray_angle = start_angle + (i * angle_step);
                float rayDirX = cosf(ray_angle);
                float rayDirY = sinf(ray_angle);

                // 1. Which grid square we are in (divide world pos by tile size)
                int mapX = (int)(player.x / TILE_SIZE);
                int mapY = (int)(player.y / TILE_SIZE);

                // 2. How far to travel to move 1 full unit in X or Y
                // We use fabsf instead of std::abs for C
                float deltaDistX = (rayDirX == 0) ? 1e30 : fabsf(1.0f / rayDirX);
                float deltaDistY = (rayDirY == 0) ? 1e30 : fabsf(1.0f / rayDirY);

                float sideDistX, sideDistY;
                int stepX, stepY;

                // 3. Calculate Step and Initial SideDist
                if (rayDirX < 0)
                {
                        stepX = -1;
                        sideDistX = (player.x / TILE_SIZE - mapX) * deltaDistX;
                }
                else
                {
                        stepX = 1;
                        sideDistX = (mapX + 1.0f - player.x / TILE_SIZE) * deltaDistX;
                }

                if (rayDirY < 0)
                {
                        stepY = -1;
                        sideDistY = (player.y / TILE_SIZE - mapY) * deltaDistY;
                }
                else
                {
                        stepY = 1;
                        sideDistY = (mapY + 1.0f - player.y / TILE_SIZE) * deltaDistY;
                }

                // 4. The DDA Loop (The "Jumping")
                int hit = 0;
                int side = 0; // 0 for X-wall, 1 for Y-wall
                while (hit == 0)
                {
                        // Jump to next grid square in the direction of the closest line
                        if (sideDistX < sideDistY)
                        {
                                sideDistX += deltaDistX;
                                mapX += stepX;
                                side = 0;
                        }
                        else
                        {
                                sideDistY += deltaDistY;
                                mapY += stepY;
                                side = 1;
                        }

                        // Check if we hit a wall or went out of bounds
                        if (mapX < 0 || mapX >= MAP_XB || mapY < 0 || mapY >= MAP_YB)
                        {
                                hit = 1;
                        }
                        else if (map[mapY][mapX] > 0)
                        {
                                hit = 1;
                        }
                }

                // 5. Calculate final distance
                // This is math magic that prevents fish-eye and uses tile units
                float wallDist;
                if (side == 0)
                        wallDist = (sideDistX - deltaDistX);
                else
                        wallDist = (sideDistY - deltaDistY);

                // 6. Draw the line back on the screen (Multiply back by TILE_SIZE to draw)
                // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                // SDL_RenderLine(renderer,
                //                player.x,
                //                player.y,
                //                player.x + rayDirX * wallDist * TILE_SIZE,
                //                player.y + rayDirY * wallDist * TILE_SIZE);
                if (wallDist <= 0) wallDist = 0.01;
                float wallH = SCREEN_HIG/wallDist;
                float wallW = SCREEN_WID/RAYS;
                float wallY = SCREEN_HIG/2 - wallDist/2;
                float wallX = i*wallW;
                SDL_FRect wall = {wallX, wallY, wallW, wallH};
                if (i%2 == 0) SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
                else if (i%3 == 0) SDL_SetRenderDrawColor(renderer, 0, 100, 100, 255);
                else if (i%5 == 0) SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
                else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &wall);
                
        }
}

void update_player(struct Player *player, struct PlayerControls playerCtrl)
{
        // roatate
        if (playerCtrl.rotLeft)
                player->angle -= player->rotSpeed;
        if (playerCtrl.rotRight)
                player->angle += player->rotSpeed;

        float moveX = cosf(player->angle) * player->speed;
        float moveY = sinf(player->angle) * player->speed;
        float nextX = 0;
        float nextY = 0;
        // movement
        if (playerCtrl.moveForward)
        {
                nextX += moveX;
                nextY += moveY;
        }
        if (playerCtrl.moveBack)
        {
                nextX -= moveX;
                nextY -= moveY;
        }
        float buffer = (nextX > 0) ? player->size : -player->size;
        int nextMapX = (int)(player->x + nextX + buffer) / TILE_SIZE;
        int currentMapY = (int)(player->y) / TILE_SIZE;

        if (map[currentMapY][nextMapX] == 0)
        {
                player->x += nextX;
        }

        buffer = (nextY > 0) ? player->size/2 : -player->size/2;
        int currentMapX = (int)(player->x) / TILE_SIZE;
        int nextMapY = (int)(player->y + nextY + buffer) / TILE_SIZE;

        if (map[nextMapY][currentMapX] == 0)
        {
                player->y += nextY;
        }
}



int main(int argc, char *argv[])
{
        if (!SDL_Init(SDL_INIT_VIDEO))
                return -1;

        SDL_Window *window;
        SDL_Renderer *renderer;
        if (!SDL_CreateWindowAndRenderer("Doom", SCREEN_WID, SCREEN_HIG, 0, &window, &renderer))
                return -2;

        struct Player player = {150, 150, 10, 3.0f, 0, 0.05f, DEG_TO_RAD(60)};
        struct PlayerControls pcontrol = {0};
        int running = 1;

        while (running)
        {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                        if (event.type == SDL_EVENT_QUIT)
                                running = 0;

                        int isDown = (event.type == SDL_EVENT_KEY_DOWN); // automatic state finding 0/1
                        if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
                        {
                                switch (event.key.key)
                                {
                                case SDLK_UP:
                                case SDLK_W:
                                        pcontrol.moveForward = isDown;
                                        break;
                                case SDLK_DOWN:
                                case SDLK_S:
                                        pcontrol.moveBack = isDown;
                                        break;
                                case SDLK_LEFT:
                                case SDLK_Q:
                                        pcontrol.rotLeft = isDown;
                                        break;
                                case SDLK_RIGHT:
                                case SDLK_E:
                                        pcontrol.rotRight = isDown;
                                        break;
                                }
                        }
                }

                update_player(&player, pcontrol);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // draw_map(renderer);
                // draw_player(renderer, player);
                draw_rays(renderer, player);

                SDL_RenderPresent(renderer);
                SDL_Delay(16);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}