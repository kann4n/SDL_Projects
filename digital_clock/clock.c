#include "clock.h"
#include <time.h>

const SDL_FColor COLOR_ON = {1.0f, 0.0f, 0.0f, 1.0f};
const SDL_FColor COLOR_OFF = {0.2f, 0.0f, 0.0f, 0.4f};

static int digits[10][7] = {
    // top, middle, bottom, top-left, top-right, bottom-right, bottom-left
    {1, 0, 1, 1, 1, 1, 1},  // 0
    {0, 0, 0, 0, 1, 1, 0},  // 1
    {1, 1, 1, 0, 1, 0, 1},  // 2
    {1, 1, 1, 0, 1, 1, 0},  // 3
    {0, 1, 0, 1, 1, 1, 0},  // 4
    {1, 1, 1, 1, 0, 1, 0},  // 5
    {1, 1, 1, 1, 0, 1, 1},  // 6
    {1, 0, 0, 0, 1, 1, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 1, 1, 0}   // 9
};

static const float ymid = SCREEN_HIG / 2.0f;
static const float spacing = 5.0f;

static SDL_FPoint digits_center_points[6] = {
    {SEGMENT_LEN * 2 - spacing, ymid},
    {SEGMENT_LEN * 3 + SEGMENT_WID * 2 + spacing, ymid},
    {SEGMENT_LEN * 6 - spacing, ymid},
    {SEGMENT_LEN * 7 + SEGMENT_WID * 2 + spacing, ymid},
    {SEGMENT_LEN * 10 - spacing, ymid},
    {SEGMENT_LEN * 11 + SEGMENT_WID * 2 + spacing, ymid}
};

void display_segment(SDL_Renderer *renderer, SDL_FPoint center, int align_vertical, int fill) {
    SDL_Vertex vertexs[6];
    SDL_FPoint points[7];

    if (!align_vertical) {
        points[0] = (SDL_FPoint){center.x - SEGMENT_LEN / 2, center.y};
        points[1] = (SDL_FPoint){center.x - SEGMENT_LEN / 3, center.y + SEGMENT_WID / 2};
        points[2] = (SDL_FPoint){center.x + SEGMENT_LEN / 3, center.y + SEGMENT_WID / 2};
        points[3] = (SDL_FPoint){center.x + SEGMENT_LEN / 2, center.y};
        points[4] = (SDL_FPoint){center.x + SEGMENT_LEN / 3, center.y - SEGMENT_WID / 2};
        points[5] = (SDL_FPoint){center.x - SEGMENT_LEN / 3, center.y - SEGMENT_WID / 2};
    } else { // change off set  in x to y and y to x
        points[0] = (SDL_FPoint){center.x, center.y - SEGMENT_LEN / 2};
        points[1] = (SDL_FPoint){center.x + SEGMENT_WID / 2, center.y - SEGMENT_LEN / 3};
        points[2] = (SDL_FPoint){center.x + SEGMENT_WID / 2, center.y + SEGMENT_LEN / 3};
        points[3] = (SDL_FPoint){center.x, center.y + SEGMENT_LEN / 2};
        points[4] = (SDL_FPoint){center.x - SEGMENT_WID / 2, center.y + SEGMENT_LEN / 3};
        points[5] = (SDL_FPoint){center.x - SEGMENT_WID / 2, center.y - SEGMENT_LEN / 3};
    }
    points[6] = points[0];

    int tris[12] = {0, 1, 5, 5, 1, 2, 5, 2, 4, 4, 2, 3}; // 3 points make triangle total 4 triangles that make geomerty
    for (int i = 0; i < 6; i++) {
        vertexs[i].position = points[i];
        vertexs[i].color = fill ? COLOR_ON : COLOR_OFF;
    }

    if (fill) {
        SDL_RenderGeometry(renderer, NULL, vertexs, 6, tris, 12);
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 0, 0, 100);
        SDL_RenderLines(renderer, points, 7);
    }
}

void display_7segment(SDL_Renderer *renderer, SDL_FPoint centerDig, int *pfilled_segments) {
    float halflen = SEGMENT_LEN / 2;
    float halfgap = GAP / 2;
    SDL_FPoint segPoss[7] = {
        {centerDig.x, centerDig.y - SEGMENT_LEN - GAP},
        {centerDig.x, centerDig.y},
        {centerDig.x, centerDig.y + SEGMENT_LEN + GAP},
        {centerDig.x - halflen - GAP, centerDig.y - halflen - halfgap},
        {centerDig.x + halflen + GAP, centerDig.y - halflen - halfgap},
        {centerDig.x + halflen + GAP, centerDig.y + halflen + halfgap},
        {centerDig.x - halflen - GAP, centerDig.y + halflen + halfgap}
    };
    int alignment[] = {0, 0, 0, 1, 1, 1, 1};
    for (int i = 0; i < 7; i++) {
        display_segment(renderer, segPoss[i], alignment[i], 0);
        if (pfilled_segments[i]) display_segment(renderer, segPoss[i], alignment[i], 1);
    }
}

void display_clock(SDL_Renderer *renderer) {
    time_t curr_time = time(NULL);
    struct tm *lt = localtime(&curr_time);
    int c_digs[6] = {lt->tm_hour/10, lt->tm_hour%10, lt->tm_min/10, lt->tm_min%10, lt->tm_sec/10, lt->tm_sec%10};
    for (int i = 0; i < 6; i++) {
        display_7segment(renderer, digits_center_points[i], digits[c_digs[i]]);
    }
}

void display_colon(SDL_Renderer *renderer, SDL_FPoint centers[2]) {
    int G = 10, S = 10;
    SDL_FRect rects[4] = {
        {centers[0].x - S/2, ymid - G, S, S}, {centers[0].x - S/2, ymid + G, S, S},
        {centers[1].x - S/2, ymid - G, S, S}, {centers[1].x - S/2, ymid + G, S, S}
    };
    SDL_SetRenderDrawColor(renderer, 250, 50, 50, 255);
    for(int i=0; i<4; i++) SDL_RenderFillRect(renderer, &rects[i]);
}
