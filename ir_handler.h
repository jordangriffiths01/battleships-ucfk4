/**
@file       ir_handler.h
@authors    Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
@date       27 SEPT 2015

@brief      IR handler headers
*/

#ifndef IR_HANDLER_H
#define IR_HANDLER_H


/** Required library modules */
#include "ir_uart.h"
#include "tinygl.h"


/** Special character to represent nothing received */
#define NO_POSITION  0xff


/** Define character encoding macros, using encoding 0b00xxxyyy */
#define ENCODE_POS(x, y) (x << 3 | y)
#define DECODE_X(encoded_pos) (encoded_pos >> 3)
#define DECODE_Y(encoded_pos) (encoded_pos & 0x7)


/** Status codes used for IR - must be in range 0x40 -> 0x7f **/
typedef enum
{
    NORESPONSE_S = 0x40,    //Used when no status code received
    PLAYER_TWO_S,           //Sent to tell other player they are player 2 (and to begin in wait phase)
    HIT_S,                  //Sent to communicate the requested strike was a hit
    MISS_S,                 //Sent to communicate the requested strike was a miss
    LOSER_S,                //Sent after players turn, to indicate they won and game is over
    PLAYON_S,               //Sent after players turn, to continue play
    PLAY_AGAIN_S            //Sent when a new game is requested

} states;


/**
Send a given status character to other player
@param status status code to be transmitted
 */
void ir_send_status(states status);


/**
Get status code being transmitted from other player (or special code for
no response).
@return received status code (or special NORESPONSE_S code if nothing received)
 */
states ir_get_status(void);


/**
Encodes position as 0b00xxxyyy and transmits as character.
@param pos tinygl_point representing position of strike
*/
void ir_send_strike(tinygl_point_t pos);


/**
Receives position transmitted from other player (as character)
@return received encoded position, or special NO_POSITION
integer if nothing received
*/
tinygl_point_t ir_decode_strike(char c);


/**
Decode a received position character 0b00xxxyyy into its x and y components
@return a tinygl_point representing this position
*/
uint8_t ir_get_position(void);



#endif
