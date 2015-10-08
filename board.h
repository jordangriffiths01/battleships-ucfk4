/** @file     board.h
    @authors  Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
    @date     27 September 2015

    @brief    Game board routine definitions for Battleships
**/

#ifndef BOARD_H
#define BOARD_H


/** Required library modules */
#include "tinygl.h"


/** Game customisation parameters */
#define NUM_SHIPS 3
#define WINNING_SCORE 9
#define SHIP_LENGTHS {2,3,4}


/** board specific enumeration definitions */
typedef enum rotation {HORIZ, VERT} rotation_t;
typedef enum dir {DIR_N, DIR_E, DIR_S, DIR_W, DIR_DOWN, DIR_NONE} dir_t;
typedef enum board_type {THIS_BOARD, TARGET_BOARD} board_type_t;
typedef enum strike_result {HIT, MISS} strike_result_t;


/** Structure definition for ship to be placed */
typedef struct ship {
    tinygl_point_t pos;
    rotation_t rot;
    int length;
} Ship;


/**
Initialise logic, gameboard and cursor positions
*/
void board_init(void);


/**
load current ship into game board, if position is valid.
@return true (1) if ship placement was successful, false (0) otherwise.
*/
bool place_ship(void);


/**
Verify that current ship position is non-overlapping with other placed ships
@return true (1) if position is valid, false (0) otherwise.
*/
bool is_valid_position(void);


/**
Verify that strike location was not a previous successful hit.
@return true (1) if position is valid, false (0) otherwise.
*/
bool is_valid_strike(void);


/**
Add successful strike location to target board and increment score
*/
void add_hit(void);


/**
Check whether enemy strike hits a ship.
@return true (1) for hit, false (0) for miss.
*/
bool is_hit(tinygl_point_t pos);


/**
Check whether current game score is such that all ships have been sunk.
@return true (1) if all enemy ships sunk, false (0) otherwise.
*/
bool is_winner(void);


/**
Move the ship currently being placed.
@param dir direction to move ship.
*/
void move_ship(dir_t dir);


/**
Move the strike cursor.
@param dir direction to move cursor.
*/
void move_cursor(dir_t dir);


/**
Accessor method for external modules to access ship being placed
@return pointer to current ship structure
*/
Ship* get_ship(void);


/**
Accessor method for external modules to access cursor position
@return cursor location
*/
tinygl_point_t get_cursor(void);


/**
Accessor method for external modules to access gameboard
@param board_type specifies which game board (this or target)
@return pointer to board bitmap
*/
uint8_t* get_board(board_type_t board_type);


/**
Rotate current ship by 90 degrees
*/
void rotate_ship(void);


/**
Reset current ship to be placed with a new length
(used after previous ship has been placed).
@param newlen length of next ship to be placed.
*/
void reset_cur_ship(uint8_t newlen);


/**
Attempts to generate next ship to be placed.
@returns false (0) if no more ships, true (1) otherwise
*/
bool next_ship(void);


#endif
