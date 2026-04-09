#ifndef DATA_H
#define DATA_H

const static int digits[10][7] = {
    // top, middle, bottom, top-left, top-right, bottom-right, bottom-left
    {1, 0, 1, 1, 1, 1, 1}, // 0
    {0, 0, 0, 0, 1, 1, 0}, // 1
    {1, 1, 1, 0, 1, 0, 1}, // 2
    {1, 1, 1, 0, 1, 1, 0}, // 3
    {0, 1, 0, 1, 1, 1, 0}, // 4
    {1, 1, 1, 1, 0, 1, 0}, // 5
    {1, 1, 1, 1, 0, 1, 1}, // 6
    {1, 0, 0, 0, 1, 1, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 1, 1, 0}  // 9
};

static const float ymid = SCREEN_HIG / 2.0f;
static const float spacing = 5.0f;

const static SDL_FPoint digits_center_points[6] = {
    {SEGMENT_LEN * 2 - spacing, ymid},
    {SEGMENT_LEN * 3 + SEGMENT_WID * 2 + spacing, ymid},
    {SEGMENT_LEN * 6 - spacing, ymid},
    {SEGMENT_LEN * 7 + SEGMENT_WID * 2 + spacing, ymid},
    {SEGMENT_LEN * 10 - spacing, ymid},
    {SEGMENT_LEN * 11 + SEGMENT_WID * 2 + spacing, ymid}
};



#endif
