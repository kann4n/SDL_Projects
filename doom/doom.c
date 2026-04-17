#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WID 800
#define SCREEN_HIG 600
#define MAP_XB 32
#define MAP_YB 32
#define TILE_SIZE 50
#define MAP_SIZE 200
#define MAPTILE_WIDTH (MAP_SIZE / MAP_XB)
#define MAPTILE_HIGHT (MAP_SIZE / MAP_YB)
#define RAY_KOUNT 200

// helpers
#define M_PI 3.14159265358979323846
#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0f)

int map[MAP_YB][MAP_XB] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 2, 2, 2, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 1},
    {1, 0, 2, 0, 0, 0, 0, 0, 2, 0, 4, 4, 4, 4, 4, 4, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 3, 0, 3, 0, 0, 1},
    {1, 0, 2, 0, 2, 2, 2, 0, 2, 0, 4, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 3, 0, 3, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 2, 0, 2, 2, 2, 0, 2, 0, 4, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 3, 3, 3, 0, 0, 1},
    {1, 0, 2, 2, 2, 0, 2, 2, 2, 0, 4, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 1},
    {1, 0, 4, 4, 4, 4, 4, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 0, 2, 0, 0, 1},
    {1, 0, 4, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 2, 2, 0, 0, 1},
    {1, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 4, 4, 0, 4, 4, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 1},
    {1, 0, 3, 3, 0, 3, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 5, 5, 0, 5, 5, 0, 0, 1},
    {1, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 4, 4, 4, 0, 0, 1},
    {1, 0, 3, 3, 3, 3, 3, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 4, 0, 4, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 5, 5, 5, 0, 5, 5, 5, 0, 2, 0, 0, 0, 0, 2, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 2, 2, 2, 0, 0, 1},
    {1, 0, 5, 0, 0, 0, 0, 0, 5, 0, 2, 0, 0, 0, 0, 2, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 2, 0, 2, 0, 0, 1},
    {1, 0, 5, 0, 5, 5, 5, 0, 5, 0, 2, 0, 0, 0, 0, 2, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 2, 2, 2, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 5, 0, 5, 5, 5, 0, 5, 0, 2, 2, 2, 2, 2, 2, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 3, 3, 3, 0, 0, 1},
    {1, 0, 5, 5, 5, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

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
        float ratioX = (float)MAP_SIZE / (MAP_XB * TILE_SIZE);
        float ratioY = (float)MAP_SIZE / (MAP_YB * TILE_SIZE);
        for (int i = 0; i < MAP_YB; i++)
        {
                for (int j = 0; j < MAP_XB; j++)
                {
                        if (map[i][j] == 1)
                                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);
                        else if (map[i][j] == 2)
                                SDL_SetRenderDrawColor(renderer, 139, 69, 15, 200);
                        else if (map[i][j] == 3)
                                SDL_SetRenderDrawColor(renderer, 180, 50, 50, 200);
                        else if (map[i][j] > 4)
                                SDL_SetRenderDrawColor(renderer, 0, 180, 185, 200);
                        else if (map[i][j] == 5)
                                SDL_SetRenderDrawColor(renderer, 40, 150, 50, 200);
                        else
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
                        SDL_FRect block = {
                            j * TILE_SIZE * ratioX,
                            i * TILE_SIZE * ratioY,
                            TILE_SIZE * ratioX,
                            TILE_SIZE * ratioY,
                        };
                        SDL_RenderFillRect(renderer, &block);
                }
        }
}

void draw_player(SDL_Renderer *renderer, struct Player player)
{
        float worldSizeX = MAP_XB * TILE_SIZE;
        float worldSizeY = MAP_YB * TILE_SIZE;

        float ratioX = (float)MAP_SIZE / worldSizeX;
        float ratioY = (float)MAP_SIZE / worldSizeY;

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
        SDL_FRect player_obj = {
            player.x * ratioX - player.size / 2,
            player.y * ratioY - player.size / 2,
            player.size,
            player.size};
        SDL_RenderFillRect(renderer, &player_obj);
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

void draw_3d(SDL_Renderer *renderer, struct Player player)
{
        const int RAYS = RAY_KOUNT; // RES - higher is good
        float start_angle = player.angle - (player.fov / 2.0f);
        float angle_step = player.fov / (float)RAYS;
        int celltype = 0;
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

                // 4. The DDA Loop
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
                                celltype = 1;
                        }
                        else if (map[mapY][mapX] > 0)
                        {
                                hit = 1;
                                celltype = map[mapY][mapX];
                        }
                }

                // 5. Calculate final distance (Corrected for Fisheye)
                float wallDist;
                if (side == 0)
                        wallDist = (sideDistX - deltaDistX);
                else
                        wallDist = (sideDistY - deltaDistY);

                // Fix Fisheye: multiply by cos of relative angle
                float ca = player.angle - ray_angle;
                if (ca < 0)
                        ca += 2 * M_PI;
                if (ca > 2 * M_PI)
                        ca -= 2 * M_PI;
                wallDist = wallDist * cosf(ca);

                // 6. Projecting to 2D Screen
                if (wallDist < 0.1f)
                        wallDist = 0.1f; // Prevent division by zero

                // wallH should be inversely proportional to distance
                float wallH = (SCREEN_HIG / wallDist);

                // Center the wall vertically
                float wallY = (SCREEN_HIG / 2.0f) - (wallH / 2.0f);
                float wallW = (float)SCREEN_WID / RAYS;
                float wallX = i * wallW;

                SDL_FRect wall = {wallX, wallY, wallW, wallH};

                if (celltype == 1)
                        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                else if (celltype == 2)
                        SDL_SetRenderDrawColor(renderer, 139, 69, 15, 255);
                else if (celltype == 3)
                        SDL_SetRenderDrawColor(renderer, 180, 50, 50, 255);
                else if (celltype > 4)
                        SDL_SetRenderDrawColor(renderer, 0, 180, 185, 255);
                else if (celltype == 5)
                        SDL_SetRenderDrawColor(renderer, 40, 150, 50, 255);
                else
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
                // xy lighting
                if (side == 1)
                {
                        Uint8 r, g, b, a;
                        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                        SDL_SetRenderDrawColor(renderer, r / 2, g / 2, b / 2, a); // dim it
                }
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

        buffer = (nextY > 0) ? player->size / 2 : -player->size / 2;
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
        // renderer config
        // set apllha mode on
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // set vsync on/off(if this is on comment out sdl_delay because this func will do it)
        SDL_SetRenderVSync(renderer, 1); // 2nd para = vsync interval

        struct Player player = {150, 150, 5, 3.0f, 0, 0.05f, DEG_TO_RAD(60)};
        struct PlayerControls pcontrol = {0};

        Uint64 lastTime = SDL_GetTicks();
        Uint64 frameKount = 0;
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
                // Draw Sky
                SDL_SetRenderDrawColor(renderer, 204, 255, 255, 255); // lifht blue sky
                SDL_FRect sky = {0, 0, SCREEN_WID, SCREEN_HIG / 2};
                SDL_RenderFillRect(renderer, &sky);

                // Draw Floor
                SDL_SetRenderDrawColor(renderer, 0, 51, 51, 255); // blueblack floor
                SDL_FRect floor = {0, SCREEN_HIG / 2, SCREEN_WID, SCREEN_HIG / 2};
                SDL_RenderFillRect(renderer, &floor);

                draw_3d(renderer, player);
                draw_map(renderer);
                draw_player(renderer, player);

                SDL_RenderPresent(renderer);

                // other things
                frameKount++;
                Uint64 currentTime = SDL_GetTicks();
                if (currentTime - lastTime > 1000)
                {
                        char title[60];
                        sprintf(title, "fps: %ld", frameKount);
                        SDL_SetWindowTitle(window, title);
                        lastTime = currentTime;
                        frameKount = 0;
                }
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}