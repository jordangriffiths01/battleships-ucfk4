/** @file     display_handler.h
    @authors  Jordan Griffiths & Jonty Trombik
    @date     27 September 2015

    @brief    display routine definitions
**/


#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDELR_H

#include "board.h"
#include "tinygl.h"

#define SCROLL_SPEED 25
#define DISPLAY_TASK_RATE 300

void initialise_display(void);

void draw_ship(void);

void draw_cursor(void);

void draw_board(board_type_t board_type);

void display_result(strike_result_t last_result);

#endif
