#include "board.h"

uint8_t ship_lengths[NUM_SHIPS] = {2,3,4};

void board_init(void) {
  int i;
  for (i = 0; i < DISPLAY_WIDTH; i++) {
    boards[THIS_BOARD][i] = 0;
    boards[TARGET_BOARD][i] = 0;
  }
  cur_ship_num = 0;
  reset_cur_ship(ship_lengths[cur_ship_num]);
}

bool place_ship(void) {
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

bool is_valid_position(void) {
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

void move_ship(dir_t dir) {
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

void move_cursor(dir_t dir) {

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


Ship* get_ship(void) { return &cur_ship; }

uint8_t* get_board(board_type_t board_type) {
  return boards[board_type];
}


void rotate_ship(void) {
  cur_ship.pos = tinygl_point(0,0);
  cur_ship.rot = (cur_ship.rot + 1) % 2;
}

void reset_cur_ship(uint8_t newlen) {
  cur_ship.length = newlen;
  cur_ship.pos = tinygl_point(0,0);
  cur_ship.rot = HORIZ;
}

bool next_ship(void) {
  cur_ship_num++;
  if (cur_ship_num == NUM_SHIPS) {
    return 0;
  } else {
    reset_cur_ship(ship_lengths[cur_ship_num]);
    return 1;
  }
}
