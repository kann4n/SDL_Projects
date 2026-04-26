#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include <math.h>

#define SCRN_WIDTH 800
#define SCRN_HEIGHT 600
#define BALL_SPEED 200.0f
#define SPEED 400.0f
#define debug 0

typedef enum
{
	Ready,
	Lost,
	Won,
	Quit
} PlayerStatus;

typedef struct
{
	SDL_FRect rect;
	SDL_Texture *texture;
	PlayerStatus status;
} Player;

typedef struct
{
	float x, y, r;
	SDL_Texture *texture;
	float vx, vy;
} Ball;

typedef struct
{
	Player left;
	Player right;
	Ball ball;
	bool running;
} Game;

typedef struct
{
	bool move_up;
	bool move_down;
} PlayerControls;

void update_player(Player *p, bool up, bool down, float dt)
{
	float dir = (float)down - (float)up;
	p->rect.y += dir * SPEED * dt;
	p->rect.y = SDL_clamp(p->rect.y, 0, SCRN_HEIGHT - p->rect.h);
}

void update_ball(Game *game, float dt)
{
	Ball *b = &game->ball;
	b->x += b->vx * dt;
	b->y += b->vy * dt;

	if (b->y - b->r < 0 || b->y + b->r > SCRN_HEIGHT)
	{
		b->vy = -b->vy;
		b->y = SDL_clamp(b->y, b->r, SCRN_HEIGHT - b->r);
	}

	SDL_FRect ball_rect = {b->x - b->r, b->y - b->r, b->r * 2.01, b->r * 2.01};

	if (SDL_HasRectIntersectionFloat(&ball_rect, &game->left.rect))
	{
		b->vx = fabsf(b->vx);
		b->x = game->left.rect.x + game->left.rect.w + b->r;
	}
	else if (SDL_HasRectIntersectionFloat(&ball_rect, &game->right.rect))
	{
		b->vx = -fabsf(b->vx); // Ensure it goes left
		b->x = game->right.rect.x - b->r;
	}

	if (b->x < 0)
	{
		game->left.status = Lost;
		game->right.status = Won;
	}
	else if (b->x > SCRN_WIDTH)
	{
		game->left.status = Won;
		game->right.status = Lost;
	}
}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	if (!SDL_CreateWindowAndRenderer("Pingo", SCRN_WIDTH, SCRN_HEIGHT, 0, &window, &renderer))
	{
		SDL_Log("Failed to create window/renderer: %s", SDL_GetError());
		return -1;
	}

	Game game = {
		.left = {
			.rect = {20, SCRN_HEIGHT * 0.25f, 1, 50},
			.texture = IMG_LoadTexture(renderer, "assets/boxer.png"),
			.status = Ready,
		},
		.right = {
			.rect = {SCRN_WIDTH - 20, SCRN_HEIGHT * 0.75f, 1, 50},
			.texture = IMG_LoadTexture(renderer, "assets/boxer.png"),
			.status = Ready,
		},
		.ball = {
			.x = SCRN_WIDTH / 2,
			.y = SCRN_HEIGHT / 2,
			.r = 10,
			.vx = BALL_SPEED,
			.vy = BALL_SPEED * 0.5f,
			.texture = IMG_LoadTexture(renderer, "assets/red-ball.png"),
		},
		.running = true,
	};
	if (!game.left.texture || !game.right.texture || !game.ball.texture)
	{
		SDL_Log("Failed to load textures");
		return -2;
	}
	PlayerControls l_ctrl = {0}, r_ctrl = {0};
	Uint64 last_time = SDL_GetTicks();
	while (game.running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				game.running = false;

			if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
			{
				bool is_down = (event.type == SDL_EVENT_KEY_DOWN);
				switch (event.key.key)
				{
				case SDLK_W:
					l_ctrl.move_up = is_down;
					break;
				case SDLK_S:
					l_ctrl.move_down = is_down;
					break;
				case SDLK_UP:
					r_ctrl.move_up = is_down;
					break;
				case SDLK_DOWN:
					r_ctrl.move_down = is_down;
					break;
				}
			}
		}

		// Timing
		Uint64 now = SDL_GetTicks();
		float dt = (now - last_time) / 1000.0f;
		last_time = now;
		if (dt > 0.05f)
			dt = 0.05f;

		// Update
		if (game.left.status != Ready || game.right.status != Ready)
		{
			continue;
		}

		update_player(&game.left, l_ctrl.move_up, l_ctrl.move_down, dt);
		update_player(&game.right, r_ctrl.move_up, r_ctrl.move_down, dt);
		update_ball(&game, dt);

		// Render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Center line
		SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
		SDL_RenderLine(renderer, SCRN_WIDTH / 2, 0, SCRN_WIDTH / 2, SCRN_HEIGHT);

		// Draw
		if (debug)
		{
			SDL_SetRenderDrawColor(renderer, 255, 25, 25, 255);
			SDL_RenderFillRect(renderer, &game.left.rect);
			SDL_RenderFillRect(renderer, &game.right.rect);
		}
		if (game.left.texture)
			SDL_RenderTexture(renderer, game.left.texture, NULL, &game.left.rect);
		if (game.right.texture)
			SDL_RenderTexture(renderer, game.right.texture, NULL, &game.right.rect);
		SDL_FRect b_rect = {
			game.ball.x - game.ball.r,
			game.ball.y - game.ball.r,
			game.ball.r * 2,
			game.ball.r * 2,
		};
		if (game.ball.texture)
			SDL_RenderTexture(renderer, game.ball.texture, NULL, &b_rect);

		if (debug)
		{
			SDL_SetRenderDrawColor(renderer, 255, 25, 25, 255);
			SDL_RenderFillRect(renderer, &b_rect);
		}

		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
}