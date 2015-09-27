#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDELR_H

#include "board.h"
#include "tinygl.h"

#define SCROLL_SPEED 11
#define DISPLAY_TASK_RATE 250

void initialise_display(void);

void draw_ship(void);

void draw_board(board_type_t board_type);
#endif