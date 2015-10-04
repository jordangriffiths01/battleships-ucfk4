/** @file     display_handler.c
    @authors  Jordan Griffiths & Jonty Trombik
    @date     27 September 2015

    @brief    Display handling routines for battleships game
**/


#include "display_handler.h"


tinygl_point_t tick_points[NUM_TICK_POINTS] = {
    {2,6}, {3,5}, {4,4}, {3,3}, {2,2}, {1,1}, {0,0}, {0,0}, {0,0},
};

uint8_t ship_bitmap[DISPLAY_WIDTH] = {0x8, 0xA, 0x2A, 0x7F, 0x3E};

uint8_t target_bitmap[3][DISPLAY_WIDTH] = {
    {0x6B, 0x41, 0x2A, 0x41, 0x6B},
    {0x63, 0x49, 0x1C, 0x49, 0x63},
    {0x63, 0x41, 0x8, 0x41, 0x63},
};

uint8_t cross_bitmap[DISPLAY_WIDTH] = {0x22, 0x14, 0x8, 0x14, 0x22};

void initialise_display(void) {
    tinygl_init(DISPLAY_TASK_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_speed_set(SCROLL_SPEED);
    tinygl_font_set (&font3x5_1);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_update();
}


void draw_ship(void) {
    Ship* ship = get_ship();
    uint8_t i;
    for (i = 0; i < ship->length; i++) {
        if (ship->rot == HORIZ) {
            tinygl_draw_point(tinygl_point(ship->pos.x + i, ship->pos.y), 1);
        } else if (ship->rot == VERT) {
            tinygl_draw_point(tinygl_point(ship->pos.x, ship->pos.y + i), 1);
        }
    }
}


void draw_cursor(void) {
    tinygl_point_t cursor = get_cursor();
    tinygl_draw_point(cursor, 1);
}


void draw_board(board_type_t board_type) {
    uint8_t *board = get_board(board_type);
    int i, j;
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT; j++) {
            uint8_t on =  board[i] & BIT(j);
            tinygl_draw_point(tinygl_point(i, j), on);
        }
    }
}


void display_result(strike_result_t last_result) {
    tinygl_clear();
    if (last_result == MISS) {
        tinygl_text("  MISS");
    } else {
        tinygl_text("  HIT");
    }
}

int draw_tick_step(void) {
    static int i = 0;
    if (i > NUM_TICK_POINTS) {
        return 0;
    }
    tinygl_draw_point(tick_points[i], ON);
    i++;
    return 1;
}

int draw_cross_step(void) {
    static int step = 0;
    if (step >= NUM_CROSS_FLASHES * 2) {
        return 0;

    } else if (step % 2 == 0) {
        int i, j;
        for (i = 0; i < DISPLAY_WIDTH; i++) {
            for (j = 0; j < DISPLAY_HEIGHT; j++) {
                tinygl_draw_point(tinygl_point(i, j), cross_bitmap[i] >> j & 1);
            }
        }
    } else {
        tinygl_clear();
    }
    step++;
    return 1;

}

void draw_target_step(void) {
    static int step = 0;
    tinygl_clear();
    int i, j;
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT; j++) {
            tinygl_draw_point(tinygl_point(i, j), target_bitmap[step][i] >> j & 1);
        }
    }
    step = (step + 1) % NUM_TARGET_STEPS;
}

void draw_ship_step(void) {
    static int step = 0;
    tinygl_clear();
    int i, j;
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT; j++) {
            tinygl_draw_point(tinygl_point(i, j), (ship_bitmap[i] << step) >> (j + DISPLAY_HEIGHT) & 1);
        }
    }
    step = (step + 1) % NUM_SHIP_STEPS;
}
