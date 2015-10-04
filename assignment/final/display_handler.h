/** @file     display_handler.h
    @authors  Jordan Griffiths & Jonty Trombik
    @date     27 September 2015

    @brief    display routine definitions
**/

#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDELR_H

#include "board.h"
#include "tinygl.h"

#define ON 1
#define OFF 0

#define SCROLL_SPEED 27
#define DISPLAY_TASK_RATE 300

#define TICK_STEP_RATE 5
#define NUM_TICK_POINTS 9

#define CROSS_STEP_RATE 2
#define NUM_CROSS_FLASHES 2

#define TARGET_STEP_RATE 4
#define NUM_TARGET_STEPS 3

#define SHIP_STEP_RATE 4
#define NUM_SHIP_STEPS 18


void initialise_display(void);

void draw_ship(void);

void draw_cursor(void);

void draw_board(board_type_t board_type);

void display_result(strike_result_t last_result);

int draw_tick_step(void);

int draw_cross_step(void);

void draw_target_step(void);

void draw_ship_step(void);

#endif
