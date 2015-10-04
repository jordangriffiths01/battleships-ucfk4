/** @file     display_handler.c
    @authors  Jordan Griffiths & Jonty Trombik
    @date     27 September 2015

    @brief    Display handling routines for battleships game
**/


#include "display_handler.h"


tinygl_point_t tick_points[NUM_TICK_POINTS] =
{
    {2,6},
    {3,5},
    {4,4},
    {3,3},
    {2,2},
    {1,1},
    {0,0},
    {0,0},
    {0,0},
};

tinygl_point_t cross_points[NUM_CROSS_POINTS] =
{
    {0,1},
    {0,5},
    {1,2},
    {1,4},
    {2,3},
    {3,2},
    {3,4},
    {4,1},
    {4,5},
};

tinygl_point_t target_points [NUM_TARGET_POINTS] =
{
    {0,0},
    {1,0},
    {3,0},
    {4,0},
    {0,1},
    {4,1},
    {2,1},
    {0,3},
    {2,3},
    {4,3},
    {2,5},
    {0,5},
    {4,5},
    {0,6},
    {1,6},
    {3,6},
    {4,6},
};

tinygl_point_t target_points_2 [NUM_TARGET_POINTS] =
{
    {0,0},
    {1,0},
    {3,0},
    {4,0},
    {0,1},
    {4,1},
    {2,2},
    {1,3},
    {2,3},
    {3,3},
    {2,4},
    {0,5},
    {4,5},
    {0,6},
    {1,6},
    {3,6},
    {4,6},
};

tinygl_point_t target_points_3 [NUM_TARGET_POINTS] =
{
    {0,0},
    {1,0},
    {3,0},
    {4,0},
    {0,1},
    {4,1},
    {2,3},
    {2,3},
    {2,3},
    {2,3},
    {2,3},
    {0,5},
    {4,5},
    {0,6},
    {1,6},
    {3,6},
    {4,6},
};

tinygl_point_t ship_points [NUM_SHIP_POINTS] =
{
    {4,1},
    {4,2},
    {4,3},
    {4,4},
    {4,5},
    {3,0},
    {3,1},
    {3,2},
    {3,3},
    {3,4},
    {3,5},
    {3,6},
    {2,1},
    {2,3},
    {2,5},
    {1,1},
    {1,3},
    {0,3},
};

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
    tinygl_draw_point(tinygl_point(1,1), 1);
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
    tinygl_draw_point(tick_points[i], 1);
    i++;
    return 1;
}

int draw_cross_step(void) {
    static int i = 0;
    if (i > NUM_CROSS_FLASHES * 2) {
        return 0;
    }
    int j;
    for (j = 0; j < NUM_CROSS_POINTS; j++) {
        tinygl_draw_point(cross_points[j], i%2);
    }
    i++;
    return 1;

}

void draw_target_step(void) {
    static int step = 0;
    tinygl_clear();
    int i;
    for (i = 0; i < NUM_TARGET_POINTS; i++) {
        if (step == 0) {tinygl_draw_point(target_points[i], 1); }
        else if (step == 1) {tinygl_draw_point(target_points_2[i], 1); }
        else if (step == 2) {tinygl_draw_point(target_points_3[i], 1); }
    }
    step = (step + 1) % 3;
}

void draw_ship_step(void) {
    static int step = 0;
    tinygl_clear();
    int i;
    for (i = 0; i < NUM_SHIP_POINTS; i++) {
        int x = ship_points[i].x;
        int y = ship_points[i].y + step - DISPLAY_HEIGHT;
        tinygl_draw_point(tinygl_point(x,y), 1);
    }
    step = (step + 1) % SHIP_STEP_NUMBER;

}
