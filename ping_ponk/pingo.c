#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <math.h>

#define SCRN_WIDTH 800
#define SCRN_HEIGHT 600
#define BALL_SPEED 200.0f
#define SPEED 400.0f
#define WIN_SCORE 10

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
	int score;
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

// helper function
void reset_ball(Game *game)
{
	game->ball.x = SCRN_WIDTH / 2;
	game->ball.y = SCRN_HEIGHT / 2;
	game->ball.vx *= (SDL_rand(2) == 0 ? 1 : -1) * 1.01f;
	game->ball.vy *= (SDL_rand(2) == 0 ? 1 : -1) * 1.01f;
}

// functions
void update_player(Player *p, bool up, bool down, float dt)
{
	float dir = (float)down - (float)up;
	p->rect.y += dir * SPEED * dt;
	p->rect.y = SDL_clamp(p->rect.y, 0, SCRN_HEIGHT - p->rect.h);
}

void update_ball(Game *game, float dt)
{
	Ball *b = &game->ball;
	// Move the ball
	b->x += b->vx * dt;
	b->y += b->vy * dt;
	if (b->y - b->r < 0 || b->y + b->r > SCRN_HEIGHT)
	{
		b->vy = -b->vy;
		b->y = SDL_clamp(b->y, b->r, SCRN_HEIGHT - b->r);
	}

	bool isatleft = b->x < SCRN_WIDTH / 2;
	SDL_FRect ballrect = {b->x - b->r, b->y - b->r, b->r * 2, b->r * 2};
	if (isatleft)
	{
		if (SDL_HasRectIntersectionFloat(&ballrect, &game->left.rect))
		{
			b->vx = -b->vx;
			b->x = game->left.rect.x + game->left.rect.w + b->r;
		}
		// check if it went past the left edge
		else if (b->x + b->r < -5) // just a little bit left more
		{
			reset_ball(game);
			game->right.score += 1;
			if (game->right.score >= WIN_SCORE)
			{
				game->right.status = Won;
				game->left.status = Lost;
			}
		}
	}
	else
	{
		if (SDL_HasRectIntersectionFloat(&ballrect, &game->right.rect))
		{
			b->vx = -b->vx;
			b->x = game->right.rect.x - b->r;
		}
		// check if it went past the right edge
		else if (b->x - b->r > SCRN_WIDTH + 5)
		{
			reset_ball(game);
			game->left.score += 1;
			if (game->left.score >= WIN_SCORE)
			{
				game->left.status = Won;
				game->right.status = Lost;
			}
		}
	}
}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	if (!TTF_Init())
	{
		SDL_Log("TTF_Init failed: %s", SDL_GetError());
		return -1;
	}
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_srand(0);
	if (!SDL_CreateWindowAndRenderer("Pingo", SCRN_WIDTH, SCRN_HEIGHT, 0, &window, &renderer))
	{
		SDL_Log("Failed to create window/renderer: %s", SDL_GetError());
		return -1;
	}

	Game game = {
		.left = {
			.rect = {20, SCRN_HEIGHT * 0.25f, 10, 50},
			.texture = IMG_LoadTexture(renderer, "assets/boxer.png"),
			.status = Ready,
			.score = 0,
		},
		.right = {
			.rect = {SCRN_WIDTH - 20, SCRN_HEIGHT * 0.75f, 10, 50},
			.texture = IMG_LoadTexture(renderer, "assets/boxer.png"),
			.status = Ready,
			.score = 0,
		},
		.ball = {
			.x = SCRN_WIDTH / 2,
			.y = SCRN_HEIGHT / 2,
			.r = 15,
			.vx = BALL_SPEED,
			.vy = BALL_SPEED * 0.5f,
			.texture = IMG_LoadTexture(renderer, "assets/red-ball.png"),
		},
		.running = true,
	};
	if (!game.left.texture || !game.right.texture || !game.ball.texture)
	{
		SDL_Log("Failed to load textures: %s", SDL_GetError());
		return -2;
	}
	TTF_Font *font = TTF_OpenFont("assets/font_pureevil.ttf", 32);
	if (!font)
	{
		SDL_Log("Font load failed: %s", SDL_GetError());
		return -3;
	}
	SDL_Texture *score_tex = NULL;
	int last_left = -1, last_right = -1;
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
		if (game.left.score != last_left || game.right.score != last_right)
		{
			if (score_tex)
				SDL_DestroyTexture(score_tex); // Clean up old texture

			char buf[32];
			SDL_snprintf(buf, sizeof(buf), "%d - %d", game.left.score, game.right.score);

			SDL_Color white = {255, 255, 255, 255};
			SDL_Surface *surf = TTF_RenderText_Blended(font, buf, 0, white);

			if (surf)
			{
				score_tex = SDL_CreateTextureFromSurface(renderer, surf);
				SDL_DestroySurface(surf);
			}

			last_left = game.left.score;
			last_right = game.right.score;
		}

		if (game.left.status == Ready && game.right.status == Ready)
		{
			update_player(&game.left, l_ctrl.move_up, l_ctrl.move_down, dt);
			update_player(&game.right, r_ctrl.move_up, r_ctrl.move_down, dt);
			update_ball(&game, dt);
		}
		else if (game.left.status == Lost || game.right.status == Lost)
		{
			SDL_Log("Game Over! %s wins!", game.left.status == Lost ? "Right Player" : "Left Player");
			game.running = false;
			continue;
		}

		// Render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Center line
		SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
		SDL_RenderLine(renderer, SCRN_WIDTH / 2, 0, SCRN_WIDTH / 2, SCRN_HEIGHT);

		// Draw
		SDL_RenderTexture(renderer, game.left.texture, NULL, &game.left.rect);
		SDL_RenderTexture(renderer, game.right.texture, NULL, &game.right.rect);
		SDL_FRect b_rect = {
			game.ball.x - game.ball.r,
			game.ball.y - game.ball.r,
			game.ball.r * 2.1f, // avoid rect vs circle collision issues
			game.ball.r * 2.1f, // pov ball look big but small rect is used for collision detection
		};
		SDL_RenderTexture(renderer, game.ball.texture, NULL, &b_rect);
		if (score_tex)
		{
			float w, h;
			SDL_GetTextureSize(score_tex, &w, &h);
			SDL_FRect dest = {(SCRN_WIDTH - w) / 2, 20, w, h};
			SDL_RenderTexture(renderer, score_tex, NULL, &dest);
		}
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(score_tex);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyTexture(game.left.texture);
	SDL_DestroyTexture(game.right.texture);
	SDL_DestroyTexture(game.ball.texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}