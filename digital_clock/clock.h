#ifndef CLOCK_H
#define CLOCK_H

#include <SDL3/SDL.h>

#define SEGMENT_LEN 50.0f
#define SEGMENT_WID 10.0f
#define GAP 4.0f
#define SCREEN_WID 670
#define SCREEN_HIG 200

enum Mode 
{
    TIMER = 't',
    STOPWATCH = 's',
    CLOCK = 'c'
};

typedef struct Time{
    int hour;
    int minute;
    int second;
} Time;

// Function prototypes
void display_segment(SDL_Renderer *renderer, SDL_FPoint center, int align_vertical, int fill);
void display_7segment(SDL_Renderer *renderer, SDL_FPoint centerDig, int *pfilled_segments);
void display_clock(SDL_Renderer *renderer, enum Mode mode, int start_timesec);
void display_colon(SDL_Renderer *renderer, SDL_FPoint centers[2]);

#endif
