/**
@file     display_handler.c
@authors  Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
@date     27 September 2015

@brief    Display handling routines for battleships game
**/


#include "display_handler.h"



/** Array defining points for tick animation (in order) */
static tinygl_point_t tick_points[NUM_TICK_POINTS] =
{
    {2,6}, {3,5}, {4,4}, {3,3}, {2,2}, {1,1}, {0,0}, {0,0}, {0,0},
};


/** Bitmap for cross icon */
static uint8_t cross_bitmap[DISPLAY_WIDTH] =
{
    0x22, 0x14, 0x8, 0x14, 0x22
};


/** Bitmap for ship icon */
static uint8_t ship_bitmap[DISPLAY_WIDTH] =
{
    0x8, 0xA, 0x2A, 0x7F, 0x3E
};


/** Array of bitmaps for target animation (one bitmap = one step) */
static uint8_t target_bitmap[NUM_TARGET_STEPS][DISPLAY_WIDTH] =
{
    {0x63, 0x49, 0x1C, 0x49, 0x63},
    {0x63, 0x41, 0x0, 0x41, 0x63},
};



/**
Initialise tinygl environment
*/
void initialise_display(void)
{
    tinygl_init(DISPLAY_TASK_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_speed_set(SCROLL_SPEED);
    tinygl_font_set (&font3x5_1);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_update();
}


/**
Draw ship currently being placed
*/
void draw_ship(void)
{
    Ship* ship = get_ship();
    uint8_t i;
    for (i = 0; i < ship->length; i++) {
        if (ship->rot == HORIZ) {
            tinygl_draw_point(tinygl_point(ship->pos.x + i, ship->pos.y), ON);
        } else if (ship->rot == VERT) {
            tinygl_draw_point(tinygl_point(ship->pos.x, ship->pos.y + i), ON);
        }
    }
}


/**
Draw cursor on display
 */
void draw_cursor(void)
{
    tinygl_point_t cursor = get_cursor();
    tinygl_draw_point(cursor, ON);
}


/**
Draw current board state.
@param board_type specifies which board to display (this or target)
 */
void draw_board(board_type_t board_type)
{
    uint8_t *board = get_board(board_type);
    int i, j;
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT; j++) {
            uint8_t is_on =  board[i] & BIT(j);
            tinygl_draw_point(tinygl_point(i, j), is_on);
        }
    }
}


/**
Display message indicating whether strike was successful
@param last_result outcome of strike (hit or miss)
 */
void display_result(strike_result_t last_result)
{
    tinygl_clear();
    if (last_result == MISS) {
        tinygl_text(" MISS");
    } else {
        tinygl_text(" HIT  ");
    }
}


/**
Draw next frame for tick animation (if frames remaining).
Animation is a tick drawn one dot at a time
@return FALSE for failure (no frames remaining) or TRUE otherwise
 */
int draw_tick_step(void)
{
    static int step = 0;
    if (step > NUM_TICK_POINTS) {
        //animation finished
        step = 0;
        return FALSE;
    }

    //draw next step
    tinygl_draw_point(tick_points[step], ON);
    step++;
    return TRUE;
}


/**
Draw next frame for cross animation (if frames remaining)
Animation is a cross that flashes.
@return FALSE for failure (no frames remaining) or TRUE otherwise
 */
int draw_cross_step(void)
{
    static int step = 0;
    if (step >= NUM_CROSS_FLASHES * 2) {
        //animation finished
        step = 0;
        return FALSE;

    } else if (step % 2 == 0) {
        //draw cross
        int i, j;
        for (i = 0; i < DISPLAY_WIDTH; i++) {
            for (j = 0; j < DISPLAY_HEIGHT; j++) {
                tinygl_draw_point(tinygl_point(i, j), cross_bitmap[i] >> j & 1);
            }
        }
    } else {
        //clear cross
        tinygl_clear();
    }

    step++;
    return TRUE;
}


/**
Draw next frame for target animation (looping).
Animation consists of cycling through frame bitmaps
 */
void draw_target_step(void)
{
    static int step = 0;
    tinygl_clear();

    int i, j;
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT; j++) {
            tinygl_draw_point(tinygl_point(i, j), target_bitmap[step][i] >> j & 1);
        }
    }

    step = (step + 1) % NUM_TARGET_STEPS;
}


/**
Draw next frame for ship animation (looping).
Animation consists of ship logo moving right to left across screen, followed
by user prompt 'Push To Start!'
 */
 void draw_ship_step(void)
 {
     static int step = 0;
     static bool text_on = 0;
     int i, j;

     if (step == 2 * DISPLAY_HEIGHT + 1) {
         // Transfer to scrolling text
         tinygl_text("  PUSH TO START!");
         text_on = TRUE;
     } else if (step == 0) {
         // Transfer to ship logo
         tinygl_clear();
         text_on = 0;
     }

     if (!text_on) {
         // Draw ship step
         for (i = 0; i < DISPLAY_WIDTH; i++) {
             for (j = 0; j < DISPLAY_HEIGHT; j++) {
                 tinygl_draw_point(tinygl_point(i, j), (ship_bitmap[i] << step) >> (j + DISPLAY_HEIGHT) & 1);
             }
         }
     }

     step = (step + 1) % NUM_SHIP_STEPS;
 }
