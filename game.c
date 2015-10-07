/** @file       game.c
    @authors    Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
    @date       27 September 2015

    @brief      BATTLESHIPS!
                This module contains the main function for
                initialisation and task scheduling.
**/

#include "game.h"


/** Game state variables */
static phase_t game_phase;
static strike_result_t last_result;
static int tick;
static int phase_tick;
static spwm_t led_flicker;


/**
Display related routines to be run before game loop
*/
static void display_task_init(void)
{
    initialise_display();
    change_phase(SPLASH);
}


/**
Navswitch related routines to be run before game loop
*/
static void navswitch_task_init(void)
{
    navswitch_init();
}


/**
Game logic tasks to be run before game loop
*/
static void game_task_init(void)
{
    board_init();
    game_phase = SPLASH;
    tick = 0;
    phase_tick = 0;
}


/**
Button related routines to be run before game loop
*/
static void button_task_init(void)
{
    button_init();
}


/**
LED related routines to be run before game loop
*/
static void led_task_init(void)
{
    led_init();
    spwm_period_set(&led_flicker, LED_PERIOD);
    spwm_duty_set(&led_flicker, LED_DUTY);
    spwm_reset(&led_flicker);
}


/**
IR related routines to be run before game loop
*/
static void ir_task_init(void)
{
    ir_uart_init();
}


/**
Handles navswitch tasks dependant on the game phase.
*/
static void navswitch_task(void)
{
    dir_t dir;
    switch(game_phase) {

        case PLACING:
            dir = get_navswitch_dir();
            if (dir >= DIR_N && dir <= DIR_W) {
                move_ship(dir);
            } else if (dir == DIR_DOWN && place_ship() && !next_ship()) {
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

        default:
            break;
    }
}


/**
Handles button tasks dependant on the current phase.
*/
static void button_task(void)
{
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

            case PLAY_AGAIN:
                ir_send_status(PLAY_AGAIN_S);
                reset_game();
                break;

            default:
                break;
        }
    }
}


/**
Handles blue LED tasks dependant on the game phase.
*/
static void led_task(void) {

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
        case RESULT_GRAPHIC:
            last_result == HIT ? led_set(LED1, spwm_update(&led_flicker)) : led_set(LED1, 0);
            break;

        case ENDRESULT:
            is_winner() ? led_set(LED1, 1) : led_set(LED1, 0);
            break;

        default:
            break;

    }

}


/**
Handles display tasks dependant on the current game phase.
*/
static void display_task(void) {
    switch (game_phase) {
        case SPLASH:
            phase_tick += 1;
            if (phase_tick > LOOP_RATE / SHIP_STEP_RATE) {
                phase_tick = 0;
                draw_ship_step();
            }
            break;

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

        case RESULT_GRAPHIC:
            phase_tick += 1;
            if (last_result == HIT && phase_tick > LOOP_RATE / TICK_STEP_RATE) {
                phase_tick = 0;
                if (!draw_tick_step()) {change_phase(RESULT); }

            } else if (last_result == MISS && phase_tick > LOOP_RATE / CROSS_STEP_RATE) {
                phase_tick = 0;
                if (!draw_cross_step()) {change_phase(RESULT); }
            }
            break;

        case WAIT:
        case TRANSFER:
            phase_tick += 1;
            if (phase_tick > LOOP_RATE / TARGET_STEP_RATE) {
                phase_tick = 0;
                draw_target_step();
            }
            break;

        default:
            break;
    }
    tinygl_update();
}


/**
Runs any IR tasks dependant on the current game phase.
*/
static void ir_task(void) {
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
                    add_hit();
                    last_result = HIT;
                    change_phase(RESULT_GRAPHIC);
                    break;

                case MISS_S:
                    last_result = MISS;
                    change_phase(RESULT_GRAPHIC);
                    break;

                default:
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
            if (status == LOSER_S) change_phase(ENDRESULT);
            else if (status == PLAYON_S)  change_phase(AIM);
            break;

        case PLAY_AGAIN:
            if(ir_get_status() == PLAY_AGAIN_S){
                reset_game();

            }
            break;

        default:
            break;
    }
}

/**
handles switching between time oriented game phases.
*/
static void game_task(void) {
    switch(game_phase) {

        case RESULT:
            phase_tick += 1;
            if (phase_tick > LOOP_RATE * RESULT_DURATION) {
                phase_tick = 0;
                if (is_winner()) {
                    ir_send_status(LOSER_S);
                    change_phase(ENDRESULT);
                } else {
                    ir_send_status(PLAYON_S);
                    change_phase(WAIT);
                }
            }
            break;

        case ENDRESULT:
            phase_tick += 1;
            if (phase_tick > LOOP_RATE * GAMEOVER_DURATION) {
                phase_tick = 0;

                change_phase(PLAY_AGAIN);

            }
            break;

        default:
            break;
    }
}


/**
Swaps states to the provided game phase.
@param new_phase game state to transfer into.
*/
void change_phase(phase_t new_phase) {
    switch (new_phase) {
        case SPLASH:
            tinygl_clear();
            //draw_ship_icon();
            //tinygl_text("  PUSH TO START");
            break;

        case READY:
            tinygl_clear();
            tinygl_text("  READY?");
            break;

        case RESULT_GRAPHIC:
            tinygl_clear();
            break;

        case RESULT:
            tinygl_clear();
            display_result(last_result); //display_handler
            break;

        case WAIT:
            tinygl_clear();
            break;

        case ENDRESULT:
            tinygl_clear();
            if (game_phase == RESULT) {
                tinygl_text("  YOU WIN!");
            } else {
                tinygl_text("  YOU LOSE!");
            }
            break;

        case PLAY_AGAIN:
            tinygl_clear();
            tinygl_text("  PUSH TO PLAY AGAIN!");
            break;

        default:
            break;
    }
    phase_tick = 0;
    game_phase = new_phase;
}


/**
Get latest navswitch event and return associated direction
*/
dir_t get_navswitch_dir(void)
{
    navswitch_update();
    if (navswitch_push_event_p (NAVSWITCH_WEST)) return DIR_W;
    if (navswitch_push_event_p (NAVSWITCH_EAST)) return DIR_E;
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) return DIR_N;
    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) return DIR_S;
    if (navswitch_push_event_p (NAVSWITCH_PUSH)) return DIR_DOWN;
    return DIR_NONE;
}


/**
Re-initializes states to re-start game.
*/
void reset_game(void)
{
    tick = 0;
    phase_tick = 0;
    board_init();
    change_phase(PLACING);
}


/**
Re-initializes states to re-start game.
*/
int main (void)
{

    system_init ();
    display_task_init ();
    button_task_init ();
    game_task_init ();
    navswitch_task_init();
    led_task_init();
    ir_task_init();

    pacer_init(LOOP_RATE);

    while(1) {
        pacer_wait();
        tick += 1;
        if (tick > LOOP_RATE / NAVSWITCH_TASK_RATE) {
            tick = 0;
            navswitch_task();
        }
        button_task();
        game_task();
        led_task();
        ir_task();
        display_task();
    }
}
