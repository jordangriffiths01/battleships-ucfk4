/** @file     game.c
    @authors  Jordan Griffiths & Jonty Trombik
    @date     27 September 2015

    BATTLESHIPS!
    This module contains header information for the primary game handler.
**/



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
#define BUTTON_TASK_RATE 20
#define GAME_TASK_RATE 20
#define NAVSWITCH_TASK_RATE 20
#define LED_TASK_RATE 20
#define IR_TASK_RATE 100

#define TEXT_DURATION 3
#define LED_PERIOD (LED_TASK_RATE / 4)
#define LED_DUTY (LED_TASK_RATE / 6)

typedef enum phase {
  SPLASH, //Used for first display message.
  PLACING, //Ship placement phase.
  READY, //Waiting for a player to choose player1.
  AIM,   //Active turn phase - player1 selects strike location.
  FIRE,  //Strike location selected, sending IR to other player.
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

/** Initializes tinygl and sets a splash screen message. */
static void display_task_init(void);

static void navswitch_task_init(void);

static void game_task_init(void);

static void button_task_init(void);

/* Initializes LED and LED flasher. */
static void led_task_init(void);

/** Initializes IR and ir_uart module */
static void ir_task_init(void);

static void navswitch_task(__unused__ void *data);

static void button_task(__unused__ void *data);

static void led_task(__unused__ void *data);

static void display_task(__unused__ void *data);

static void ir_task(__unused__ void *data);

static void game_task(__unused__ void *data);

/** Switches the current game phase to the provided enum value */
void change_phase(phase_t new_phase);

dir_t get_navswitch_dir(void);


#endif
