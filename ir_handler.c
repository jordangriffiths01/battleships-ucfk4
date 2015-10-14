/**
@file       ir_handler.c
@authors    Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
@date       27 SEPT 2015

@brief      IR handling and message manipulation
*/

#include "ir_handler.h"


/** Received character variable */
static int8_t rcvChar;


/**
Send a given status character to other player
@param status status code to be transmitted
 */
void ir_send_status(states status)
{
    ir_uart_putc(status);
}


/**
Get status code being transmitted from other player (or special code for
no response).
@return received status code (or special NORESPONSE_S code if nothing received)
 */
states ir_get_status(void)
{
    if (ir_uart_read_ready_p ()) {
        rcvChar =  ir_uart_getc();
        return rcvChar;
    }

    return NORESPONSE_S;
}


/**
Encodes position as 0b00xxxyyy and transmits as character.
@param pos tinygl_point representing position of strike
*/
void ir_send_strike(tinygl_point_t pos)
{
    char msg = ENCODE_POS(pos.x, pos.y);
    ir_uart_putc(msg);
}


/**
Receives position transmitted from other player (as character)
@return received encoded position, or special NO_POSITION
integer if nothing received
*/
uint8_t ir_get_position(void)
{
    if (ir_uart_read_ready_p ()) {
        char rcvdChar =  ir_uart_getc();
        return (uint8_t) rcvdChar;
    }

    return NO_POSITION;
}


/**
Decode a received position character 0b00xxxyyy into its x and y components
@return a tinygl_point representing this position
*/
tinygl_point_t ir_decode_strike(char pos)
{
    uint8_t x = DECODE_X(pos);
    uint8_t y = DECODE_Y(pos);
    return tinygl_point(x, y);
}
