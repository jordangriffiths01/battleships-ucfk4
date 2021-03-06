/**
@file     display_handler.h
@authors  Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
@date     27 September 2015

@brief    Display Routine Definitions
**/

#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDELR_H


/** Required Library Modules */
#include "board.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"


/** LED display macros */
#define ON 1
#define OFF 0


/** Display parameters */
#define SCROLL_SPEED 27
#define DISPLAY_TASK_RATE 300


/** Animation parameters */
#define TICK_STEP_RATE 10
#define NUM_TICK_POINTS 9

#define CROSS_STEP_RATE 2
#define NUM_CROSS_FLASHES 2

#define TARGET_STEP_RATE 3
#define NUM_TARGET_STEPS 2

#define SHIP_STEP_RATE 4
#define NUM_SHIP_STEPS 40


/**
Initialise tinygl environment
*/
void initialise_display(void);


/**
Draw ship currently being placed
*/
void draw_ship(void);


/**
Draw cursor
 */
void draw_cursor(void);


/**
Draw current board state.
@param board_type specifies which board to display (this or target)
 */
void draw_board(board_type_t board_type);


/**
Display message indicating whether strike was successful
@param last_result outcome of strike (hit or miss)
 */
void display_result(strike_result_t last_result);


/**
Draw next frame for tick animation (if frames remaining).
Animation is a tick drawn one dot at a time
@return FALSE for failure (no frames remaining) or TRUE otherwise
 */
int draw_tick_step(void);


/**
Draw next frame for cross animation (if frames remaining)
Animation is a cross that flashes.
@return FALSE for failure (no frames remaining) or TRUE otherwise
 */
int draw_cross_step(void);


/**
Draw next frame for target animation (looping).
Animation consists of cycling through frame bitmaps
 */
void draw_target_step(void);


/**
Draw next frame for ship animation (looping).
Animation consists of ship logo moving right to left across screen
 */
void draw_ship_step(void);


#endif
