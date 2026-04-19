#include <SDL3/SDL.h>


#define SCRN_W 1200
#define SCRN_H 900
#define DENSITY 100
#define BOX_Y (SCRN_H*0.75)
#define WALL_THICK 5

SDL_Window *window;
SDL_Renderer *renderer;

struct Box
{
	double x, vx, side_len, mass;
};

struct Box small_box = {SCRN_W*0.25, 0, 30, 1};
struct Box big_box = {SCRN_W*0.50, -2, 60, 10000};
SDL_FRect sidewall = {SCRN_W*0.1, 0, WALL_THICK, SCRN_H*0.76};
SDL_FRect ground = {SCRN_W*0.1, SCRN_H*0.751, SCRN_W, WALL_THICK};
unsigned int counter = 0;

void draw_box(struct Box);
void update_boxes(double);

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	if(!SDL_CreateWindowAndRenderer("3141", SCRN_W, SCRN_H, 0, &window, &renderer))
		return -1;

	SDL_SetRenderVSync(renderer, 1);

	int running = 1;
	while(running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				running = 0; // stop running
		}
		// clearScrn
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		// update
		double steps = 100000; // incress for higher mass | fine grading
		for (double i=0; i<steps; i++)
			update_boxes(steps);
		
		// draw wall
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_RenderFillRect(renderer, &sidewall);
		SDL_RenderFillRect(renderer, &ground);
		SDL_RenderLine(renderer, 0, SCRN_H, sidewall.x, ground.y);
		// draw
		draw_box(small_box);
		draw_box(big_box);
		
		// draw counter
		char buffer[128];
		int scale = 4;
		SDL_snprintf(buffer, sizeof(buffer), "%u", counter);	
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_SetRenderScale(renderer, scale, scale);
		SDL_RenderDebugText(renderer, (sidewall.x + 100)/scale, (100)/scale, buffer);
		SDL_SetRenderScale(renderer, 1, 1);
		// update screen
		SDL_RenderPresent(renderer);
	}
	// free it
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void draw_box(struct Box box)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_FRect boxRect = {box.x, BOX_Y - box.side_len, box.side_len, box.side_len};
	SDL_RenderFillRect(renderer, &boxRect);
}

void update_boxes(double steper)
{
	small_box.x += small_box.vx/steper;
	big_box.x += big_box.vx/steper;
	if (small_box.x < sidewall.x)
	{
		small_box.x = sidewall.x;
		small_box.vx *= -1;
		counter++;
	}
	if (small_box.x + small_box.side_len > big_box.x) // means both touched
	{
		double total_mass = small_box.mass + big_box.mass;
		double m1 = small_box.mass;
		double v1 = small_box.vx;
		double m2 = big_box.mass;
		double v2 = big_box.vx;
		
		double overlap = small_box.x + small_box.side_len - big_box.x;
		small_box.x -= overlap/2;
		big_box.x += overlap/2;
		small_box.vx = ((m1-m2)*v1 + 2*m2*v2) / total_mass;
		big_box.vx = ((m2-m1)*v2 + 2*m1*v1) / total_mass;
		counter++;
	}
}
