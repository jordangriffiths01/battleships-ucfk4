/** @file     game.c
    @authors  Jordan Griffiths & Jonty Trombik
    @date     27 September 2015

    BATTLESHIPS!
    This module contains the main function for
    initialisation and task scheduling.
**/

#include "game.h"

static void display_task_init(void) {
  initialise_display();
  tinygl_text("  PUSH TO START");
}

static void navswitch_task_init(void) {
  navswitch_init();
}

static void game_task_init(void) {
   board_init();
   game_phase = SPLASH;
   tick = 0;
   start_tick = 0;
}

static void button_task_init(void) {
  button_init();
}

static void led_task_init(void) {
  led_init();
  flicker_on = 0;
  spwm_period_set(&led_flicker, led_period);
  spwm_duty_set(&led_flicker, led_duty);
  spwm_reset(&led_flicker);
}

static void ir_task_init(void) {
  ir_uart_init();
}

static void navswitch_task(__unused__ void *data) {

  dir_t dir;
  switch(game_phase) {

    case PLACING:
      dir = get_navswitch_dir();
      if (dir >= DIR_N && dir <= DIR_W) {
        move_ship(dir);
      } else if (dir == DIR_DOWN && place_ship() && !next_ship()) {
        ir_send_status(READY_S);
        change_phase(READY);
      }
      break;

    case AIM:
      dir = get_navswitch_dir();
      if (dir >= DIR_N && dir <= DIR_W) {
        move_cursor(dir);
      } else if(dir == DIR_DOWN && is_valid_strike()){
        ir_send_strike(get_cursor());
        change_phase(FIRE);
      }
      break;
  }
}


static void button_task(__unused__ void *data) {
  button_update();
  if (button_push_event_p(BUTTON1)) {
    switch (game_phase) {
      case SPLASH:
        change_phase(PLACING);
        break;

      case PLACING:
        rotate_ship();
        break;

      case READY:
        ir_send_status(PLAYER_TWO_S);
        change_phase(AIM);
        break;

      case RESULT:
        change_phase(WAIT);
        break;
    }
  }
}



static void led_task(__unused__ void *data) {

  switch (game_phase) {
    case PLACING:
    case AIM:
      led_set(LED1, 1);
      break;

    case SPLASH:
    case READY:
    case WAIT:
      led_set(LED1, 0);
      break;

    case RESULT:
      flicker_on ? led_set(LED1, spwm_update(&led_flicker)) : led_set(LED1, 0);
      break;
  }
}



static void display_task(__unused__ void *data) {
  switch (game_phase) {

    case PLACING:
      tinygl_clear();
      draw_board(THIS_BOARD);
      draw_ship();
      break;

    case AIM:
      tinygl_clear();
      draw_board(TARGET_BOARD);
      draw_cursor();
      break;

  }
  tinygl_update();
}



static void ir_task(__unused__ void *data) {
  states status;
  uint8_t position;
  switch (game_phase){

    case READY:
      if (ir_get_status() == PLAYER_TWO_S) { change_phase(WAIT); }
      break;

    case FIRE:
      status = ir_get_status();
      switch (status) {
        case HIT_S:
          flicker_on = 1;
          add_hit();
          last_result = HIT;
          change_phase(RESULT);
          break;

        case MISS_S:
          last_result = MISS;
          change_phase(RESULT);
          break;
      }
      break;

    case WAIT:
      position = ir_get_position();
      if (position != NO_POSITION) {
        tinygl_point_t shot = ir_decode_strike(position);
        if (is_hit(shot)) {
          ir_send_status(HIT_S);
        } else {
          ir_send_status(MISS_S);
        }
        change_phase(TRANSFER);
      }
      break;

    case TRANSFER:
      status = ir_get_status();
      if (status == LOSER_S) { change_phase(ENDRESULT); }
      else if (status == PLAYON_S) { change_phase(AIM); }
      break;

  }
}


static void game_task(__unused__ void *data) {
  tick += 1;

  switch(game_phase) {

    case RESULT:
      start_tick += 1;
      if (start_tick > GAME_TASK_RATE * TEXT_DURATION) {
        start_tick = 0;
        if (is_winner()) {
          ir_send_status(LOSER_S);
          change_phase(ENDRESULT);
        } else {
          ir_send_status(PLAYON_S);
          change_phase(WAIT);
        }
      }
  }
}



void change_phase(phase_t new_phase) {
  switch (new_phase) {
    case SPLASH:
      tinygl_clear();
      tinygl_text("  PUSH TO START");
      break;

    case READY:
      tinygl_clear();
      tinygl_text("  READY");
      break;

    case RESULT:
      tinygl_clear();
      display_result(last_result);
      break;

    case WAIT:
      tinygl_clear();
      tinygl_text("  WAIT FOR OTHER PLAYER");
      break;

    case ENDRESULT:
      tinygl_clear();
      if (game_phase == RESULT) {
        tinygl_text("  WINNER!");
      } else {
        tinygl_text("  LOSER!");
      }
      break;
  }
  game_phase = new_phase;
}

dir_t get_navswitch_dir(void) {

  navswitch_update();
  if (navswitch_push_event_p (NAVSWITCH_WEST)) return DIR_W;
  if (navswitch_push_event_p (NAVSWITCH_EAST)) return DIR_E;
  if (navswitch_push_event_p (NAVSWITCH_NORTH)) return DIR_N;
  if (navswitch_push_event_p (NAVSWITCH_SOUTH)) return DIR_S;
  if (navswitch_push_event_p (NAVSWITCH_PUSH)) return DIR_DOWN;
  return DIR_NONE;
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = navswitch_task, .period = TASK_RATE / NAVSWITCH_TASK_RATE},
        {.func = button_task, .period = TASK_RATE / BUTTON_TASK_RATE},
        {.func = game_task, .period = TASK_RATE / GAME_TASK_RATE},
        {.func = led_task, .period = TASK_RATE / LED_TASK_RATE},
        {.func = ir_task, .period = TASK_RATE / IR_TASK_RATE},
    };

    system_init ();
    display_task_init ();
    button_task_init ();
    game_task_init ();
    navswitch_task_init();
    led_task_init();
    ir_task_init();


    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}
