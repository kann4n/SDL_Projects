#include "clock.h"
#include "data.h"
#include <time.h>

SDL_FColor SegOn = {1, 0, 0, 255};
SDL_FColor SegOff = {0, 0, 0, 255};

void display_segment(SDL_Renderer *renderer, SDL_FPoint center, int align_vertical, int fill)
{
    SDL_Vertex vertexs[6];
    SDL_FPoint points[7];

    if (!align_vertical)
    {
        points[0] = (SDL_FPoint){center.x - SEGMENT_LEN / 2, center.y};
        points[1] = (SDL_FPoint){center.x - SEGMENT_LEN / 3, center.y + SEGMENT_WID / 2};
        points[2] = (SDL_FPoint){center.x + SEGMENT_LEN / 3, center.y + SEGMENT_WID / 2};
        points[3] = (SDL_FPoint){center.x + SEGMENT_LEN / 2, center.y};
        points[4] = (SDL_FPoint){center.x + SEGMENT_LEN / 3, center.y - SEGMENT_WID / 2};
        points[5] = (SDL_FPoint){center.x - SEGMENT_LEN / 3, center.y - SEGMENT_WID / 2};
    }
    else
    { // change off set  in x to y and y to x
        points[0] = (SDL_FPoint){center.x, center.y - SEGMENT_LEN / 2};
        points[1] = (SDL_FPoint){center.x + SEGMENT_WID / 2, center.y - SEGMENT_LEN / 3};
        points[2] = (SDL_FPoint){center.x + SEGMENT_WID / 2, center.y + SEGMENT_LEN / 3};
        points[3] = (SDL_FPoint){center.x, center.y + SEGMENT_LEN / 2};
        points[4] = (SDL_FPoint){center.x - SEGMENT_WID / 2, center.y + SEGMENT_LEN / 3};
        points[5] = (SDL_FPoint){center.x - SEGMENT_WID / 2, center.y - SEGMENT_LEN / 3};
    }
    points[6] = points[0];

    int tris[12] = {0, 1, 5, 5, 1, 2, 5, 2, 4, 4, 2, 3}; // 3 points make triangle total 4 triangles that make geomerty
    for (int i = 0; i < 6; i++)
    {
        vertexs[i].position = points[i];
        vertexs[i].color = fill ? SegOn : SegOff;
    }
    SDL_RenderGeometry(renderer, NULL, vertexs, 6, tris, 12);
}

void display_7segment(SDL_Renderer *renderer, SDL_FPoint centerDig,const int *pfilled_segments)
{
    float halflen = SEGMENT_LEN / 2;
    float halfgap = GAP / 2;
    SDL_FPoint segPoss[7] = {
        {centerDig.x, centerDig.y - SEGMENT_LEN - GAP},
        {centerDig.x, centerDig.y},
        {centerDig.x, centerDig.y + SEGMENT_LEN + GAP},
        {centerDig.x - halflen - GAP, centerDig.y - halflen - halfgap},
        {centerDig.x + halflen + GAP, centerDig.y - halflen - halfgap},
        {centerDig.x + halflen + GAP, centerDig.y + halflen + halfgap},
        {centerDig.x - halflen - GAP, centerDig.y + halflen + halfgap}};
    int alignment[] = {0, 0, 0, 1, 1, 1, 1};
    for (int i = 0; i < 7; i++)
    {
        display_segment(renderer, segPoss[i], alignment[i], 0);
        if (pfilled_segments[i])
            display_segment(renderer, segPoss[i], alignment[i], 1);
    }
}

void display_clock(SDL_Renderer *renderer, enum Mode mode, int start_timesec)
{
    static time_t timer_start = 0;
    if (mode == CLOCK)
    {
        time_t curr_time = time(NULL);
        struct tm *lt = localtime(&curr_time);
        int c_digs[6] = {
            lt->tm_hour / 10, lt->tm_hour % 10, 
            lt->tm_min / 10, lt->tm_min % 10, 
            lt->tm_sec / 10, lt->tm_sec % 10
        };
        for (int i = 0; i < 6; i++)
        {
            display_7segment(renderer, digits_center_points[i], digits[c_digs[i]]);
        }
    }
    else if (mode == STOPWATCH)
    {
        // Initialize start time once when we switch to this mode
        if (timer_start == 0) timer_start = time(NULL);

        double timepassed = difftime(time(NULL), timer_start);
        int hours = (int)timepassed / 3600;
        int mins = ((int)timepassed / 60) % 60;
        int secs = (int)timepassed % 60;

        int c_digs[6] = {
            hours / 10, hours % 10, 
            mins / 10,  mins % 10, 
            secs / 10,  secs % 10
        };

        for (int i = 0; i < 6; i++)
            display_7segment(renderer, digits_center_points[i], digits[c_digs[i]]);
    }
    else if (mode == TIMER)
    {
        // Initialize start time once when we switch to this mode
        if (timer_start == 0) timer_start = time(NULL);

        double timepassed = difftime(time(NULL), timer_start);
        double remaining_time = start_timesec - timepassed;
        if (remaining_time < 0)
        {
            remaining_time = 0;
            SDL_Log("Timer finished!");
            return;
        }
        int hours = (int)remaining_time / 3600;
        int mins = ((int)remaining_time / 60) % 60;
        int secs = (int)remaining_time % 60;

        int c_digs[6] = {
            hours / 10, hours % 10, 
            mins / 10,  mins % 10, 
            secs / 10,  secs % 10
        };
        // todo: check out -neg time
        for (int i = 0; i < 6; i++)
            display_7segment(renderer, digits_center_points[i], digits[c_digs[i]]);
    }
    else
    {
        SDL_Log("Unknown mode: %c", mode);
        return;
    }
}

void display_colon(SDL_Renderer *renderer, SDL_FPoint centers[2])
{
    int G = 10, S = 10;
    SDL_FRect rects[4] = {
        {centers[0].x - S / 2, ymid - G, S, S}, {centers[0].x - S / 2, ymid + G, S, S}, {centers[1].x - S / 2, ymid - G, S, S}, {centers[1].x - S / 2, ymid + G, S, S}};
    SDL_SetRenderDrawColor(renderer, 250, 50, 50, 255);
    for (int i = 0; i < 4; i++)
        SDL_RenderFillRect(renderer, &rects[i]);
}
