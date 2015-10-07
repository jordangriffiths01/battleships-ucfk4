/** @file       board.c
    @authors    Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
    @date       27 September 2015

    @brief      Game board initialisation and manipulation routines
                for Battleships
**/

#include "board.h"


/** Define ship lengths array at runtime */
uint8_t ship_lengths[NUM_SHIPS] = SHIP_LENGTHS;


/**
Initialise logic, gameboard and cursor positions
*/
void board_init(void)
{
    int i;
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        boards[THIS_BOARD][i] = 0;
        boards[TARGET_BOARD][i] = 0;
    }
    cur_ship_num = 0;
    game_score = 0;
    reset_cur_ship(ship_lengths[cur_ship_num]);
    cursor = tinygl_point(2,3);
    strike_position = tinygl_point(0,0);
}


/**
load current ship into game board, if position is valid.
@return true (1) if ship placement was successful, false (0) otherwise.
*/
bool place_ship(void)
{
    if (is_valid_position()) {
        uint8_t i;
        for (i = 0; i < cur_ship.length; i++) {
            if (cur_ship.rot == HORIZ) {
                boards[THIS_BOARD][cur_ship.pos.x + i] |= BIT(cur_ship.pos.y);
            } else if (cur_ship.rot == VERT) {
                boards[THIS_BOARD][cur_ship.pos.x] |= BIT(cur_ship.pos.y + i);
            }
        }
        return 1;
    } else {
        return 0;
    }
}


/**
Verify that current ship position is non-overlapping with other placed ships
@return true (1) if position is valid, false (0) otherwise.
*/
bool is_valid_position(void)
{
    uint8_t i;
    for (i = 0; i < cur_ship.length; i++) {
        if (cur_ship.rot == HORIZ && boards[THIS_BOARD][cur_ship.pos.x + i] & BIT(cur_ship.pos.y)) {
            return 0;
        } else if (cur_ship.rot == VERT && boards[THIS_BOARD][cur_ship.pos.x] & BIT(cur_ship.pos.y + i)) {
            return 0;
        }
    }
    return 1;
}


/**
Verify that strike location was not a previous successful hit.
@return true (1) if position is valid, false (0) otherwise.
*/
bool is_valid_strike()
{
    strike_position = tinygl_point(cursor.x, cursor.y);
    return !(boards[TARGET_BOARD][cursor.x] >> cursor.y & 1);
}


/**
Add successful strike location to target board and increment score
*/
void add_hit(void)
{
    boards[TARGET_BOARD][strike_position.x] |= BIT(strike_position.y);
    game_score += 1;
}


/**
Check whether enemy strike hits a ship.
@return true (1) for hit, false (0) for miss.
*/
bool is_hit(tinygl_point_t pos)
{
    return (boards[THIS_BOARD][pos.x] >> pos.y) & 1;
}


/**
Check whether current game score is such that all ships have been sunk.
@return true (1) if all enemy ships sunk, false (0) otherwise.
*/
bool is_winner(void)
{
    return game_score == WINNING_SCORE;
}


/**
Move the ship currently being placed.
@param dir direction to move ship.
*/
void move_ship(dir_t dir)
{
    uint8_t x_offset = cur_ship.rot == HORIZ? cur_ship.length - 1:0;
    uint8_t y_offset = cur_ship.rot == VERT? cur_ship.length - 1:0;

    if (dir == DIR_W) {
        cur_ship.pos.x -= cur_ship.pos.x  == 0? 0 : 1;
    } else if (dir == DIR_E) {
        cur_ship.pos.x += cur_ship.pos.x + x_offset == DISPLAY_WIDTH - 1? 0 : 1;
    } else if (dir == DIR_N) {
        cur_ship.pos.y -= cur_ship.pos.y == 0? 0 : 1;
    } else if (dir == DIR_S) {
        cur_ship.pos.y += cur_ship.pos.y + y_offset == DISPLAY_HEIGHT - 1? 0 : 1;
    }
}


/**
Move the strike cursor.
@param dir direction to move cursor.
*/
void move_cursor(dir_t dir)
{

    if (dir == DIR_W) {
        cursor.x -= cursor.x  == 0? 0 : 1;
    } else if (dir == DIR_E) {
        cursor.x += cursor.x == DISPLAY_WIDTH - 1? 0 : 1;
    } else if (dir == DIR_N) {
        cursor.y -= cursor.y == 0? 0 : 1;
    } else if (dir == DIR_S) {
        cursor.y += cursor.y == DISPLAY_HEIGHT - 1? 0 : 1;
    }
}


/**
Accessor method for external modules to access ship being placed
@return pointer to current ship structure
*/
Ship* get_ship(void)
{
    return &cur_ship;
}


/**
Accessor method for external modules to access cursor position
@return cursor location
*/
tinygl_point_t get_cursor(void)
{
    return cursor;
}


/**
Accessor method for external modules to access gameboard
@param board_type specifies which game board (this or target)
@return pointer to board bitmap
*/
uint8_t* get_board(board_type_t board_type)
{
    return boards[board_type];
}


/**
Rotate current ship by 90 degrees
*/
void rotate_ship(void)
{
    cur_ship.pos = tinygl_point(0,0);
    cur_ship.rot = (cur_ship.rot + 1) % 2;
}


/**
Reset current ship to be placed with a new length
(used after previous ship has been placed).
@param newlen length of next ship to be placed.
*/
void reset_cur_ship(uint8_t newlen)
{
    cur_ship.length = newlen;
    cur_ship.pos = tinygl_point(2,3);
    cur_ship.rot = VERT;
}


/**
Attempts to generate next ship to be placed.
@returns false (0) if no more ships, true (1) otherwise
*/
bool next_ship(void)
{
    cur_ship_num++;
    if (cur_ship_num == NUM_SHIPS) {
        return 0;
    } else {
        reset_cur_ship(ship_lengths[cur_ship_num]);
        return 1;
    }
}
