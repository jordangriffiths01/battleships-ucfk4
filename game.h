/**
@file       game.h
@authors    Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
@date       27 September 2015

@brief      BATTLESHIPS!
            Header file for the primary game module.
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
#define NAVSWITCH_TASK_RATE 20
#define LOOP_RATE 300


/* Define aesthetic parameters.  */
#define RESULT_DURATION 2.2             //Duration of HIT/MISS screen (seconds)
#define GAMEOVER_DURATION 9             //Duration of WIN/LOSE screen (seconds)
#define LED_PERIOD (LOOP_RATE / 4)      //LED time on per duty cycle (clicks)
#define LED_DUTY (LOOP_RATE / 6)        //LED length of duty cycle (clicks)


/** Define game phases */
typedef enum phase {
    SPLASH,         //Used for first display message.
    PLACING,        //Ship placement phase.
    READY,          //Waiting for a player to choose player1.
    AIM,            //Active turn phase - player1 selects strike location.
    FIRE,           //Strike location selected, sending IR to other player.
    RESULT_GRAPHIC, //Display an animated graphic representing hit or miss.
    RESULT,         //Hit or miss message shown to active player
    TRANSFER,        //Next turn intermediate phase
    WAIT,           //Player 2 phase, inactive state waiting for IR.
    ENDRESULT,      //Game over message
    PLAY_AGAIN,     //Prompts user for restart
} phase_t;


/** Game state variables */
static phase_t game_phase;              //Current game phase
static strike_result_t last_result;     //Result of this players last strike
static int tick;                        //Game loop tick counter
static int phase_tick;                  //Seperate tick counter for use within a phase
static spwm_t led_flicker;              //LED modulation interface


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
static void navswitch_task(void);


/**
Handles button tasks dependant on the current phase.
*/
static void button_task(void);


/**
Handles blue LED tasks dependant on the game phase.
*/
static void led_task(void);


/**
Handles display tasks dependant on the current game phase.
*/
static void display_task(void);


/**
Runs any IR tasks dependant on the current game phase.
*/
static void ir_task(void);


/**
Handles switching between time oriented game phases.
*/
static void game_task(void);


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
