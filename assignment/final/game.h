#ifndef GAME_H
#define GAME_H



#include "system.h"
#include "task.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "tinygl.h"
#include "spwm.h"

#include "board.h"
#include "display_handler.h"
#include "ir_handler.h"

/* Define polling rates in Hz.  */
#define BUTTON_TASK_RATE 100
#define GAME_TASK_RATE 100
#define NAVSWITCH_TASK_RATE 100
#define LED_TASK_RATE 100
#define IR_TASK_RATE 100
#define TEXT_DURATION 5


typedef enum phase {
  SPLASH, //Used for first display message
  PLACING, //Ship placement phase
  READY, //Waiting for a player to choose player1
  AIM,   //Active turn phase
  FIRE,  //Fire placement selected, sending IR to other player
  RESULT, //Hit or miss message shown to active player
  TRANSFER, //Next turn intermediate phase
  WAIT,    //Player 2 phase, inactive state waiting for IR.
  ENDRESULT  //Game over message
 } phase_t;

 static phase_t game_phase;
 static strike_result_t last_result;
 static int tick;
 static int start_tick;
 static spwm_t led_flicker;
 static short flicker_on; //actiaveted when 1 (when a a shot is successful. and hit is displayed)

static void display_task_init(void);

static void navswitch_task_init(void);

static void game_task_init(void);

static void button_task_init(void);

static void led_task_init(void);

static void ir_task_init(void);

static void navswitch_task(__unused__ void *data);

static void button_task(__unused__ void *data);

static void led_task(__unused__ void *data);

static void display_task(__unused__ void *data);

static void ir_task(__unused__ void *data);

static void game_task(__unused__ void *data);

void change_phase(phase_t new_phase);

dir_t get_navswitch_dir(void);

void change_phase(phase_t new_phase);

dir_t get_navswitch_dir(void);

#endif
