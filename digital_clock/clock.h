#ifndef CLOCK_H
#define CLOCK_H

#include <SDL3/SDL.h>

// Config
#define SEGMENT_LEN 50.0f   // be > WID
#define SEGMENT_WID 15.0f   // be < LEN/2
#define GAP 5.0f            // be < WID/4 
#define SCREEN_WID 670      // be > (LEN+WID)*6
#define SCREEN_HIG 300      // be > LEN*3

enum Mode 
{
    TIMER = 't',
    STOPWATCH = 's',
    CLOCK = 'c'
};

enum State
{
    STOPPED = -1,
    PAUSED = 0,
    RUNNING = 1
};

typedef struct Time{
    int hour;
    int minute;
    int second;
} Time;

typedef struct Clock{
    Time time;
    int start_timesec;
    enum State state; // -1 = stopped, 0 = paused, 1 = running
    enum Mode mode;
} Clock;

// Function prototypes
void display_segment(SDL_Renderer *renderer, SDL_FPoint center, int align_vertical, int fill);
void display_7segment(SDL_Renderer *renderer, SDL_FPoint centerDig, const int *pfilled_segments);
void display_clock(SDL_Renderer *renderer, enum Mode mode, int start_timesec);
void display_colon(SDL_Renderer *renderer, SDL_FPoint centers[2]);

#endif
