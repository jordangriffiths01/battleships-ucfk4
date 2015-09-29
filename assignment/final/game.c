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
  spwm_period_set(&led_flicker, 20);
  spwm_duty_set(&led_flicker, 13);
  spwm_reset(&led_flicker);
}

static void ir_task_init(void) {
  ir_uart_init();
}

static void navswitch_task(__unused__ void *data) {

  if (game_phase == PLACING || game_phase == AIM) {
    navswitch_update();
    if (navswitch_push_event_p (NAVSWITCH_WEST))
    {
        if (game_phase == PLACING) { move_ship(DIR_W); }
        if (game_phase == AIM) { move_cursor(DIR_W); }
    }
    else if (navswitch_push_event_p (NAVSWITCH_EAST))
    {
        if (game_phase == PLACING) { move_ship(DIR_E); }
        if (game_phase == AIM) { move_cursor(DIR_E); }
    }
    else if (navswitch_push_event_p (NAVSWITCH_NORTH))
    {
        if (game_phase == PLACING) { move_ship(DIR_N); }
        if (game_phase == AIM) { move_cursor(DIR_N); }
    }
    else if (navswitch_push_event_p (NAVSWITCH_SOUTH))
    {
        if (game_phase == PLACING) { move_ship(DIR_S); }
        if (game_phase == AIM) { move_cursor(DIR_S); }
    }
    else if (navswitch_push_event_p (NAVSWITCH_PUSH))
    {
      if (game_phase == PLACING) {
        uint8_t success = place_ship();
        if (success && !next_ship()) {
          tinygl_clear();
          tinygl_text("  READY");
          ir_send_status(READY_S);
          game_phase = READY;
        }
      }
      else if(game_phase == AIM){
        if(is_valid_strike()){
          ir_send_strike(get_cursor());
          game_phase = FIRE;
        }
      }
    }
  }
}

static void button_task(__unused__ void *data) {
  button_update();
  if (button_push_event_p(BUTTON1)) {
    if (game_phase == SPLASH) { game_phase = PLACING; }
    else if (game_phase == PLACING) { rotate_ship(); }
    else if (game_phase == READY) {
      game_phase = AIM;
      ir_send_status(PLAYER_TWO_S);
    } else if (game_phase == RESULT) {
      tinygl_clear();
      tinygl_text("  WAIT FOR OTHER PLAYER");
      game_phase = WAIT;
    }
  }
}

static void led_task(__unused__ void *data) {
  if (game_phase == PLACING || game_phase == AIM) {
    led_set(LED1, 1);
  } else if (game_phase == SPLASH || game_phase == READY || game_phase == WAIT) {
    led_set(LED1, 0);

  }
  else if(game_phase == RESULT){
    flicker_on ? led_set(LED1, spwm_update(&led_flicker)) : led_set(LED1, 0);
  }

}

static void display_task(__unused__ void *data) {
  if (game_phase == PLACING) {
    tinygl_clear();
    draw_board(THIS_BOARD);
    draw_ship();
  } else if (game_phase == AIM) {
    tinygl_clear();
    draw_board(TARGET_BOARD);
    draw_cursor();
  }
  tinygl_update();
}

static void ir_task(__unused__ void *data) {
  if (game_phase == READY) {
    if (ir_get_status() == PLAYER_TWO_S) {
      tinygl_text("  WAIT FOR OTHER PLAYER");
      game_phase = WAIT;
    }
  }

  else if(game_phase == FIRE){
    states status = ir_get_status();
    if(status == HIT_S || status == MISS_S){
      tinygl_clear();
      if(status == HIT_S){
        tinygl_text("  HIT");
        flicker_on = 1;
        add_hit();
        // if(is_winner()) { ir_send_status(LOSER_S); }
        // else { ir_send_status(PLAYON_S); }
      }
      else if(status == MISS_S){
        tinygl_text("  MISS");
        flicker_on = false;
        // ir_send_status(PLAYON_S);
      }
      game_phase = RESULT;
    }
  }

  else if (game_phase == WAIT){
    uint8_t position = ir_get_position();
    if (position != NO_POSITION){
      tinygl_point_t shot = ir_decode_strike(position);
      if (is_hit(shot)) {
        ir_send_status(HIT_S);
        game_phase = TRANSFER;
      } else {
        ir_send_status(MISS_S);
      }
      game_phase = TRANSFER;
    }
  }

  else if (game_phase == TRANSFER) {
    uint8_t status = ir_get_status();
    if(status == LOSER_S) {
      tinygl_clear();
      tinygl_text("  LOSER");
      game_phase = ENDRESULT;
    } else if (status == PLAYON_S) {
      game_phase = AIM;
    }
  }
}

static void game_task(__unused__ void *data) {
  tick += 1;
  if (game_phase == RESULT) {
    start_tick += 1;
    if (start_tick > (GAME_TASK_RATE) * 5) {
      start_tick = 0;
      tinygl_clear();
      if(is_winner()) {
        ir_send_status(LOSER_S);
        tinygl_text("  WINNER");
        game_phase = ENDRESULT;
      } else {
        ir_send_status(PLAYON_S);
        tinygl_text("  WAIT FOR OTHER PLAYER");
        game_phase = WAIT;
      }
    }
  }
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
