
#include "system.h"
#include "task.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "tinygl.h"

#include "board.h"
#include "display_handler.h"
#include "ir_handler.h"

/* Define polling rates in Hz.  */
#define BUTTON_TASK_RATE 100
#define GAME_TASK_RATE 100
#define NAVSWITCH_TASK_RATE 100
#define LED_TASK_RATE 100
#define IR_TASK_RATE 100

typedef enum phase {SPLASH, PLACING, READY, BOTH_READY, AIM, FIRE, RESULT, TRANSFER, WAIT, WINNER} phase_t;

static phase_t game_phase;


static void display_task_init(void) {
  initialise_display();
  tinygl_text("PUSH TO START");
}

static void navswitch_task_init(void) {
  navswitch_init();
}

static void game_task_init(void) {
   board_init();
   game_phase = SPLASH;
}

static void button_task_init(void) {
  button_init();
}

static void led_task_init(void) {
  led_init();
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
            tinygl_text("READY");
            game_phase = READY;
          }
        }
    }
  }
}

static void button_task(__unused__ void *data) {
  if (game_phase == SPLASH || game_phase == PLACING) {
    button_update();
    if (button_push_event_p(BUTTON1)) {
      if (game_phase == SPLASH) { game_phase = PLACING; }
      else if (game_phase == PLACING) { rotate_ship(); }
      else if (game_phase == READY) { game_phase = AIM; } //TEMPORARY
      else if (game_phase == BOTH_READY) {
        ir_send_status(PLAYER_TWO_S);
        game_phase = AIM;
      }
    }
  }
}

static void led_task(__unused__ void *data) {
  if (game_phase == PLACING ||game_phase == BOTH_READY || game_phase == AIM) {
    led_set(LED1, 1);
  } else if (game_phase == SPLASH || game_phase == READY) {
    led_set(LED1, 0);
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
    ir_send_status(READY_S);
    if (ir_get_status() == READY) { game_phase = BOTH_READY; }
  } else if (game_phase == BOTH_READY) {
    ir_send_status(READY_S);
    if (ir_get_status() == PLAYER_TWO_S) { game_phase = WAIT; }
  }
}

static void game_task(__unused__ void *data) {

}


int main (void)
{
    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = button_task, .period = TASK_RATE / NAVSWITCH_TASK_RATE},
        {.func = game_task, .period = TASK_RATE / GAME_TASK_RATE},
        {.func = navswitch_task, .period = TASK_RATE / BUTTON_TASK_RATE},
        {.func = led_task, .period = TASK_RATE / LED_TASK_RATE},
        {.func = ir_task, .period = TASK_RATE / IR_TASK_RATE},

    };

    system_init ();

    display_task_init ();
    button_task_init ();
    game_task_init ();
    navswitch_task_init();
    led_task_init();


    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}
