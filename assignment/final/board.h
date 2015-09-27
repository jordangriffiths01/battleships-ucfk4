#ifndef BOARD_H
#define BOARD_H

#include "tinygl.h"
#include "../fonts/font3x5_1.h"

#define NUM_SHIPS 3

typedef enum rotation {HORIZ, VERT} rotation_t;
typedef enum dir {DIR_N, DIR_E, DIR_S, DIR_W} dir_t;
typedef enum board_type {THIS_BOARD, TARGET_BOARD} board_type_t;


typedef struct ship {
  tinygl_point_t pos;
  rotation_t rot;
  int length;
} Ship;

static uint8_t boards[2][DISPLAY_WIDTH];

static Ship cur_ship;
static uint8_t cur_ship_num;


void board_init(void);

bool place_ship(void);

bool is_valid_position(void);

void move_ship(dir_t dir);

Ship* get_ship(void);

uint8_t* get_board(board_type_t board_type);

void rotate_ship(void);

void reset_cur_ship(uint8_t newlen);

bool next_ship(void);

#endif