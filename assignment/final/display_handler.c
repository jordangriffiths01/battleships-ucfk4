/** @file     display_handler.c
    @authors  Jordan Griffiths & Jonty Trombik
    @date     27 September 2015

    @brief    Display handling routines for battleships game
**/


#include "display_handler.h"


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
