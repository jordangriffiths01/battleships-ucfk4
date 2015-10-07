/** @file     game.h
    @authors  Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
    @date     27 September 2015

    BATTLESHIPS!
    This module contains header information for the primary game handler.
**/

#ifndef GAME_H
#define GAME_H


/** Library Modules */
#include "system.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "tinygl.h"
#include "spwm.h"
#include "pacer.h"


/** Application Modules */
#include "board.h"
#include "display_handler.h"
#include "ir_handler.h"


/* Define polling rates in Hz.  */
#define NAVSWITCH_TASK_RATE 10
#define LOOP_RATE 300

#define RESULT_DURATION 2.5
#define GAMEOVER_DURATION 6
#define LED_PERIOD (LOOP_RATE / 4)
#define LED_DUTY (LOOP_RATE / 6)


/** Define game phases */
typedef enum phase {
    SPLASH, //Used for first display message.
    PLACING, //Ship placement phase.
    READY, //Waiting for a player to choose player1.
    AIM,   //Active turn phase - player1 selects strike location.
    FIRE,  //Strike location selected, sending IR to other player.
    RESULT_GRAPHIC, //Display an animated graphic representing hit or miss.
    RESULT, //Hit or miss message shown to active player
    TRANSFER, //Next turn intermediate phase
    WAIT,    //Player 2 phase, inactive state waiting for IR.
    ENDRESULT,  //Game over message
    PLAY_AGAIN
} phase_t;


/** Game state variables */
static phase_t game_phase;
static strike_result_t last_result;
static int tick;
static int phase_tick;
static int player;
static spwm_t led_flicker;


/**
Display related routines to be run before game loop
*/
static void display_task_init(void);


/**
Navswitch related routines to be run before game loop
*/
static void navswitch_task_init(void);


/**
Game logic tasks to be run before game loop
*/
static void game_task_init(void);


/**
Button related routines to be run before game loop
*/
static void button_task_init(void);


/**
LED related routines to be run before game loop
*/
static void led_task_init(void);


/**
IR related routines to be run before game loop
*/
static void ir_task_init(void);


/**
Handles navswitch tasks dependant on the game phase.
*/
static void navswitch_task();


/**
Handles button tasks dependant on the current phase.
*/
static void button_task();


/**
Handles blue LED tasks dependant on the game phase.
*/
static void led_task();


/**
Handles display tasks dependant on the current game phase.
*/
static void display_task();


/**
Runs any IR tasks dependant on the current game phase.
*/
static void ir_task();


/**
handles switching between time oriented game phases.
*/
static void game_task();


/**
Swaps states to the provided game phase enum value.
@param new_phase game state to transfer into.
*/
void change_phase(phase_t new_phase);


/**
Re-initializes states to re-start game.
*/
void reset_game(void);


/**
Get latest navswitch event and return associated direction
*/
dir_t get_navswitch_dir(void);

#endif
